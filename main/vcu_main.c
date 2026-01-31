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
#include "driver/gpio.h"


// initialize all hardware
void config_gpio(){
    // AI2 is ignition switch
    // GPIO_NUM_35
    
    //
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    

}

void app_main(){
    printf("[*] GuduVCU start-up procedure...");

    // dump I/O settings
    gpio_dump_io_configuration(stdout, SOC_GPIO_VALID_GPIO_MASK);
    
    fflush(stdout);
    return;
}