
idf_component_register(
    SRCS 
        "WIFI_manager.c"

    INCLUDE_DIRS "."
    REQUIRES driver nvs_flash esp_wifi
)