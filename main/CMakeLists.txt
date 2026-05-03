idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "."
                       REQUIRES esp_adc OLED_driver OLED_Data GetBatteryLevel BSP system_oled_show RTC_time WIFI_manager
                       )  

add_compile_options(-fdiagnostics-color=always)
                    
