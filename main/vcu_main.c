// entry point
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "spud.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_adc/adc_continuous.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_log.h"

// initialize all hardware
void config_gpio(){

    /* configure our six relay switch GPIO pins SW1-5 */
    gpio_config_t sw_io_conf = {};
    sw_io_conf.intr_type = GPIO_INTR_DISABLE;
    sw_io_conf.mode = GPIO_MODE_OUTPUT;
    sw_io_conf.pin_bit_mask = (1<<SW0) | (1<<SW1) | (1<<SW2) | (1<<SW3) | (1<<SW4) | (1<<SW5);
    sw_io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    sw_io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&sw_io_conf);

    /* digital output pins DO0-3 */
    gpio_config_t do_conf = {};
    do_conf.intr_type = GPIO_INTR_DISABLE;
    do_conf.mode = GPIO_MODE_INPUT_OUTPUT_OD;
    do_conf.pin_bit_mask = (1<<DO0) | (1<<DO1) | (1<<DO2) | (1<<DO3);
    do_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    do_conf.pull_up_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&do_conf);

    /* analog big boiis */
    /*
    gpio_config_t aio_conf = {};
    aio_conf.intr_type = GPIO_INTR_DISABLE;
    aio_conf.mode = GPIO_MODE_INPUT;
    aio_conf.pin_bit_mask = (1<<AI0)|(1<<AI1)|(1<<AI2)|(1<<AI3)|(1<<AI4)|(1<<AI5);
    aio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    aio_conf.pull_up_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&aio_conf);
    */
    // ADC1 (AI0, AI1, AI2, etc.)
    //adc1_config_width(ADC_WIDTH_BIT_12);
    //adc_

}

static int adc_raw[2][10];
const static char *TAG = "EXAMPLE";
void app_main(){
    printf("[*] GuduVCU start-up procedure...");

    
    config_gpio();
    
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    // ADC_CHANNEL_7 should be pin 35
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_7, &config));
    // ADC CHANNEL_0\5 should be pin 33
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_5, &config));


    
    while (1){
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_7, &adc_raw[0][0]));
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_5, &adc_raw[0][1]));

        ESP_LOGI(TAG, "ADC%d Channel[%d] Raw Data: %d\t ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, ADC_CHANNEL_7, adc_raw[0][0],
        ADC_UNIT_1+1, ADC_CHANNEL_5, adc_raw[0][1]
        );
        fflush(stdout);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // dump I/O settings
    //gpio_dump_io_configuration(stdout, SOC_GPIO_VALID_GPIO_MASK);
    
    //fflush(stdout);
    return;
}