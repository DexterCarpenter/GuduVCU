#include "esp_log.h"
#include "mcp2515.h"
#include "driver/spi_master.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"

// so this is SPI2
static const char *TAG = "CANBUS_TEST";
static const char *msg = "pong";
//void * pvParameters
//void ping_pong_loop(spi_device_handle_t *handle){
void ping_pong_loop(void *handle){

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
    devcfg.clock_speed_hz = 10000000;
    devcfg.spics_io_num = 15; // cs
    devcfg.queue_size = 128;


    
    // spin up SPI2 interface
    ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &mcp2515_spi_handle);
    ESP_ERROR_CHECK(ret);

  MCP2515 mcp2515(&mcp2515_spi_handle);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();


  struct can_frame read_frame = {};
  struct can_frame write_frame = {};
  while (true){
   ESP_LOGI(TAG, "running write/read loop");
   // write boii
   write_frame.can_id = 0x1337;
   write_frame.can_dlc=5;
   for (uint8_t i = 0; i < 5; i++){
     write_frame.data[i] = (uint8_t)msg[i];
   }

   MCP2515::ERROR err = mcp2515.sendMessage(&write_frame);
   if (err != MCP2515::ERROR_OK){
     ESP_LOGW(TAG, "could not send write frame :(");
   }

    
   // read
   if (mcp2515.readMessage(&read_frame) == MCP2515::ERROR_OK) {
       // frame contains received message
       ESP_LOGI(TAG, "got msg: %s", read_frame.data);
   }

   vTaskDelay(pdMS_TO_TICKS(5000));
  }
}


extern "C" void app_main(){

    ESP_LOGI(TAG, "big boii loading");
    // alright!
    
    
    
    /* 
        At this point, we should have an SPI connection to our MCP2515 IC.
        We should now attempt communication over the connection for CAN.
    */
    // should actually tune stack size later
    xTaskCreate(ping_pong_loop, "pingpong", 5000, NULL, 10, NULL);


    return;
}