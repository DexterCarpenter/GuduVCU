idf_component_register(SRCS "bms.c"
                       PRIV_REQUIRES spi_flash esp_adc esp_driver_spi
                       REQUIRES esp_driver_gpio
                       INCLUDE_DIRS "../../include" "../mcp-can")
                       