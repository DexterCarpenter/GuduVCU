idf_component_register(SRCS "vcu_main.c"
                       PRIV_REQUIRES spi_flash
                       REQUIRES esp_driver_gpio
                       INCLUDE_DIRS "../include")
                       