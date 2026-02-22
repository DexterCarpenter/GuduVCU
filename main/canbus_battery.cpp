#include "esp_log.h"
#include "mcp2515.h"
#include "driver/spi_master.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "spud.h"


// pls work?
static adc_oneshot_unit_handle_t adc1_handle;

SemaphoreHandle_t IGN_STATE_SEMAPHORE;


static const char *TAG = "BATTERY_TEST";
//static const char *msg = "pong";
//void * pvParameters
//void ping_pong_loop(spi_device_handle_t *handle){
void battery_listener(void *handle){
    esp_err_t ret;

    // MCP2515 is our IC for CANBUS Handling
    spi_device_handle_t mcp2515_spi_handle;

    // Configuration for the SPI 2 bus
    
    spi_bus_config_t buscfg = {
        // done with macros, using literals for this test
        .mosi_io_num = 13,
        .miso_io_num = 12,
        .sclk_io_num = 14,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0, // no limit
    };
    
    //Configuration for the SPI device on the other side of the bus
    spi_device_interface_config_t devcfg = {};
    devcfg.command_bits = 0;
    devcfg.address_bits = 0;
    devcfg.duty_cycle_pos = 128;
    devcfg.cs_ena_posttrans = 3;
    devcfg.mode = 0;
    // 8 Mhz
    devcfg.clock_speed_hz = 8 * 1000 * 1000;
    devcfg.spics_io_num = 2;//15; // cs 15 = can 0?
    devcfg.queue_size = 128;


    
    // spin up SPI2 interface
    ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &mcp2515_spi_handle);
    ESP_ERROR_CHECK(ret);

  MCP2515 mcp2515(&mcp2515_spi_handle);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();


  struct can_frame read_frame = {};


  struct can_frame write_frame = {};
  struct can_frame write_frame1 = {};
  
  while (true){
    if(xSemaphoreTake(IGN_STATE_SEMAPHORE, (TickType_t) 0) == pdTRUE){
        ESP_LOGI(TAG, "running read loop");
        // request frame
        for (uint8_t i = 0; i < 8; i++){
            write_frame.data[i]=0;
            write_frame1.data[i]=0;
        }

    write_frame.can_id = 0x79B;
    write_frame.can_dlc = 8;
    write_frame1.can_id = 0x79B;
    write_frame1.can_dlc = 8;
    write_frame.data[0] = 0x02;
    write_frame.data[1] = 0x21;
    write_frame.data[2] = 0x01;
    write_frame1.data[0]= 0x30;
    


    
    // write msg
    
   MCP2515::ERROR err = mcp2515.sendMessage(&write_frame);
   if (err != MCP2515::ERROR_OK){
     ESP_LOGW(TAG, "could not send write frame :(");
   
        err = mcp2515.sendMessage(&write_frame1);
    if (err != MCP2515::ERROR_OK){
        ESP_LOGW(TAG, "could not send write frame :(");
    }
   }
    // read
    for (int i=0; i<8; i++){
        if (mcp2515.readMessage(&read_frame) == MCP2515::ERROR_OK) {
            // frame contains received message
            // bit hack
            uint64_t payload = 0;
            memcpy(&payload, read_frame.data, sizeof(payload));
           ESP_LOGI(TAG, "got msg: id=%08x len=%d msg=%llx\n", read_frame.can_id, read_frame.can_dlc, payload);
        } else {
            ESP_LOGW(TAG, "no msg :(");
        }
    }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

static const int THRESHOLD = 500;
static const char *IGN_TAG = "IGNITION_TASK";

void prechage_sequence(){
    // PRE-CHARGE CYCLE
    ESP_LOGI(IGN_TAG, "starting pre-charge cycle");
    // NEGATIVE UP
    ESP_ERROR_CHECK(gpio_set_level(SW3, 1));
    // wait 1 second
    vTaskDelay(pdMS_TO_TICKS(1000));
    // precharge goes up
    ESP_ERROR_CHECK(gpio_set_level(SW4, 1));
    // wait 1 second
    vTaskDelay(pdMS_TO_TICKS(1000));
    // positive relay can go up
    ESP_ERROR_CHECK(gpio_set_level(SW5, 1));
    // immediately drop pre-charge relay
    ESP_ERROR_CHECK(gpio_set_level(SW4, 0));
    ESP_LOGI(IGN_TAG, "completed pre-charge cycle");

}
void ignition_task(void *handle){
    bool precharged = false;
    while (1){
        int adc_reading = 0;
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_7, &adc_reading));

        ESP_LOGI(IGN_TAG, "read adc value %d", adc_reading);
        if (adc_reading > THRESHOLD){
            // write to SW0
            
            ESP_ERROR_CHECK(gpio_set_level(SW0, 1));
            if (precharged == false){
                prechage_sequence();
                precharged = true;
            }
            vTaskDelay(pdMS_TO_TICKS(3000));
            xSemaphoreGive(IGN_STATE_SEMAPHORE);
        } else {
            ESP_ERROR_CHECK(gpio_set_level(SW0, 0));
            // positive drop first
            ESP_ERROR_CHECK(gpio_set_level(SW5, 0));
            // wait 250ms
            vTaskDelay(pdMS_TO_TICKS(250));
            // then drop negative
            ESP_ERROR_CHECK(gpio_set_level(SW3, 0));
            precharged=false;
            
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}



extern "C" void app_main(){

    ESP_LOGI(TAG, "Loading main func...");

    /* configure our six relay switch GPIO pins SW1-5 */
    gpio_config_t sw_io_conf = {};
    sw_io_conf.intr_type = GPIO_INTR_DISABLE;
    sw_io_conf.mode = GPIO_MODE_OUTPUT;
    sw_io_conf.pin_bit_mask = (1<<SW0) | (1<<SW1) | (1<<SW2) | (1<<SW3) | (1<<SW4) | (1<<SW5);
    sw_io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    sw_io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&sw_io_conf);

    
    ESP_LOGI(TAG, "done with pre-charge cycle");



    /*
        INPUT + OUTPUT CONFIG
    */
    // setup analog connection
    // GPIO 35 "A2" is actually ADC channel ADC1_CH7
    
    // we will need to have the analog handling centralized somewhere
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_7, &config));

    // semaphore
    IGN_STATE_SEMAPHORE = xSemaphoreCreateBinary();

    if (IGN_STATE_SEMAPHORE == NULL){
        ESP_LOGE(TAG, "Could not create critical semaphore in start up.  Killing program.");
        return;
    }

    // should actually tune stack size later

    xTaskCreate(ignition_task, "ignition task", 8192, NULL, 9, NULL);
    xTaskCreate(battery_listener, "battery CAN listener", 5000, NULL, 10, NULL);

    return;
}