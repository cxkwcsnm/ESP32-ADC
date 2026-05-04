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

typedef struct {
    const char* ssid;
    const char* password;
} wifi_connect_params_t;

/**
 * @brief       将Wi-Fi初始化为STA模式并扫描附近的WiFi网络
 * @param       无
 * @retval      ESP_OK on success, ESP_FAIL on failure
 */
esp_err_t wifi_scan(void);

/********************以下是一些辅助函数********************/

/**
 * @brief       连接到指定WiFi网络
 * @param       ssid: WiFi网络名称
 * @param       password: WiFi网络密码
 * @retval      ESP_OK on success, ESP_FAIL on failure
 */
void wifi_connect_task(void* params);

/**
 * @brief       检查WiFi连接状态
 * @param       无
 * @retval      true if connected, false otherwise
 */
bool wifi_is_connected(void);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_MANAGER_H */