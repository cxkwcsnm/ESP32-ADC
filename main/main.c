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
#include "WIFI_manager.h"

void app_main(void)
{
     esp_err_t ret;
    uint8_t x = 0;

    ret = nvs_flash_init();     /* 初始化NVS */
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }
    
    ESP_ERROR_CHECK(myiic_init());
    ESP_ERROR_CHECK(battery_adc_init());
    ESP_ERROR_CHECK(oled_init());
    ESP_ERROR_CHECK(RTC_init());
    ESP_ERROR_CHECK(wifi_scan());
    ESP_ERROR_CHECK(wifi_connect("DESKTOP-HTLNPUV 4127", "88888888"));

    xTaskCreate(OLEDShowTask, "OLEDShowTask", 4096, NULL, 5, NULL);
}
