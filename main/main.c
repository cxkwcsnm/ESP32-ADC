#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

#include "GetBatteryLevel.h"
#include "OLED_driver.h"
#include "OLED_Data.h"

#include "system_batteryshow.h"

void app_main(void)
{
    // 1. 初始化
    ESP_ERROR_CHECK(battery_adc_init());
    ESP_ERROR_CHECK(oled_init(I2C_NUM_0, GPIO_NUM_20, GPIO_NUM_21));

    xTaskCreate(BatteryShow, "BatteryShowTask", 4096, NULL, 5, NULL);
    
}
