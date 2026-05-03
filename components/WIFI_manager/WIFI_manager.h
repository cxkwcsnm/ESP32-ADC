#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "string.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief       将Wi-Fi初始化为STA模式并扫描附近的WiFi网络
 * @param       无
 * @retval      ESP_OK on success, ESP_FAIL on failure
 */
esp_err_t wifi_scan(void);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_MANAGER_H */