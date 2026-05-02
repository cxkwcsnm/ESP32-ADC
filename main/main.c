#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

#include "GetBatteryLevel.h"
#include "OLED_driver.h"
#include "OLED_Data.h"
#include "RTC_time.h"
#include "myiic.h"

#include "system_oled_show.h"

void app_main(void)
{
    ESP_ERROR_CHECK(myiic_init());
    ESP_ERROR_CHECK(battery_adc_init());
    ESP_ERROR_CHECK(oled_init());
    ESP_ERROR_CHECK(RTC_init());

    xTaskCreate(OLEDShowTask, "OLEDShowTask", 4096, NULL, 5, NULL);
}
