
idf_component_register(
    SRCS 
        "system_batteryshow.c"

    INCLUDE_DIRS "."
    REQUIRES OLED_Data driver GetBatteryLevel BSP OLED_driver
)