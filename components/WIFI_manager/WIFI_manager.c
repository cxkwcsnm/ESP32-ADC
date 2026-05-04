#include "WIFI_manager.h"

/* 存储12个WIFI名称 */
#define DEFAULT_SCAN_LIST_SIZE 12
static const char *TAG = "scan";

static bool s_wifi_connected = false;
static char s_wifi_ssid[32] = {0};
static char s_wifi_password[64] = {0};

/**
 * @brief       身份认证模式
 * @param       authmode :身份验证模式
 * @retval      无
 */
static void print_auth_mode(int authmode)
{
    switch (authmode)
    {
    case WIFI_AUTH_OPEN:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_OPEN");
        break;
    case WIFI_AUTH_OWE:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_OWE");
        break;
    case WIFI_AUTH_WEP:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WEP");
        break;
    case WIFI_AUTH_WPA_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA_PSK");
        break;
    case WIFI_AUTH_WPA2_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_PSK");
        break;
    case WIFI_AUTH_WPA_WPA2_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA_WPA2_PSK");
        break;
    case WIFI_AUTH_ENTERPRISE:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_ENTERPRISE");
        break;
    case WIFI_AUTH_WPA3_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA3_PSK");
        break;
    case WIFI_AUTH_WPA2_WPA3_PSK:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_WPA3_PSK");
        break;
    case WIFI_AUTH_WPA3_ENTERPRISE:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA3_ENTERPRISE");
        break;
    case WIFI_AUTH_WPA2_WPA3_ENTERPRISE:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA2_WPA3_ENTERPRISE");
        break;
    case WIFI_AUTH_WPA3_ENT_192:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_WPA3_ENT_192");
        break;
    default:
        ESP_LOGI(TAG, "Authmode \tWIFI_AUTH_UNKNOWN");
        break;
    }
}

/**
 * @brief       打印WIFI密码类型
 * @param       pairwise_cipher :密码类型
 * @param       group_cipher    :群密码类型
 * @retval      无
 */
static void print_cipher_type(int pairwise_cipher, int group_cipher)
{
    switch (pairwise_cipher)
    {
    case WIFI_CIPHER_TYPE_NONE:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_NONE");
        break;
    case WIFI_CIPHER_TYPE_WEP40:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_WEP40");
        break;
    case WIFI_CIPHER_TYPE_WEP104:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_WEP104");
        break;
    case WIFI_CIPHER_TYPE_TKIP:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_TKIP");
        break;
    case WIFI_CIPHER_TYPE_CCMP:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_CCMP");
        break;
    case WIFI_CIPHER_TYPE_TKIP_CCMP:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_TKIP_CCMP");
        break;
    case WIFI_CIPHER_TYPE_AES_CMAC128:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_AES_CMAC128");
        break;
    case WIFI_CIPHER_TYPE_SMS4:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_SMS4");
        break;
    case WIFI_CIPHER_TYPE_GCMP:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_GCMP");
        break;
    case WIFI_CIPHER_TYPE_GCMP256:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_GCMP256");
        break;
    default:
        ESP_LOGI(TAG, "Pairwise Cipher \tWIFI_CIPHER_TYPE_UNKNOWN");
        break;
    }

    switch (group_cipher)
    {
    case WIFI_CIPHER_TYPE_NONE:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_NONE");
        break;
    case WIFI_CIPHER_TYPE_WEP40:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_WEP40");
        break;
    case WIFI_CIPHER_TYPE_WEP104:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_WEP104");
        break;
    case WIFI_CIPHER_TYPE_TKIP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_TKIP");
        break;
    case WIFI_CIPHER_TYPE_CCMP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_CCMP");
        break;
    case WIFI_CIPHER_TYPE_TKIP_CCMP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_TKIP_CCMP");
        break;
    case WIFI_CIPHER_TYPE_SMS4:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_SMS4");
        break;
    case WIFI_CIPHER_TYPE_GCMP:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_GCMP");
        break;
    case WIFI_CIPHER_TYPE_GCMP256:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_GCMP256");
        break;
    default:
        ESP_LOGI(TAG, "Group Cipher \tWIFI_CIPHER_TYPE_UNKNOWN");
        break;
    }
}

/**
 * @brief       将Wi-Fi初始化为sta并设置扫描方法
 * @param       无
 * @retval      无
 */
esp_err_t wifi_scan(void)
{
    char lcd_buff[100] = {0};
    /* 网卡初始化 */
    ESP_ERROR_CHECK(esp_netif_init());
    /* 创建新的事件循环 */
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    /* 用户初始化STA模式 */
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);
    /* wifi配置初始化 */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_LOGI(TAG, "WiFi 初始化成功");

    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));
    /* 设置WIFI为STA模式 */
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    /* 启动WIFI */
    ESP_ERROR_CHECK(esp_wifi_start());
    /* 开始扫描附件的WIFI */
    esp_wifi_scan_start(NULL, true);
    /* 获取上次扫描中找到的AP数量 */
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    /* 获取上次扫描中找到的AP列表 */
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));

    ESP_LOGI(TAG, "Total APs scanned = %u", ap_count);
    /* 下面是打印附近的WIFI信息 */
    for (int i = 0; (i < DEFAULT_SCAN_LIST_SIZE) && (i < ap_count); i++)
    {
        sprintf(lcd_buff, "%s", ap_info[i].ssid);
        // spilcd_show_string(200, 20 * i, 240, 16, 16, lcd_buff, BLUE);
        ESP_LOGI(TAG, "%s", lcd_buff);
        // ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
        // ESP_LOGI(TAG, "RSSI \t\t%d", ap_info[i].rssi);
        // print_auth_mode(ap_info[i].authmode);

        /*
        if (ap_info[i].authmode != WIFI_AUTH_WEP)
        {
            print_cipher_type(ap_info[i].pairwise_cipher, ap_info[i].group_cipher);
        }
        */

        ESP_LOGI(TAG, "Channel \t\t%d\n", ap_info[i].primary);
    }
    return ESP_OK;
}

// WiFi 事件处理函数
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED)
    {
        s_wifi_connected = true;
        ESP_LOGI(TAG, "WiFi 连接成功");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        s_wifi_connected = false;
        ESP_LOGI(TAG, "WiFi 连接断开，正在尝试重新连接...");
        // 自动重新连接
        esp_wifi_connect();
    }
}

// 连接到指定 WiFi
void wifi_connect_task(void *params)
{
    wifi_connect_params_t *p = (wifi_connect_params_t *)params;

    // 保存 WiFi 配置用于重连
    strlcpy(s_wifi_ssid, p->ssid, sizeof(s_wifi_ssid));
    strlcpy(s_wifi_password, p->password, sizeof(s_wifi_password));

    // 注册事件处理
    // 这行代码把 wifi_event_handler 函数"注册"到了 ESP-IDF 的 事件系统 中。相当于告诉系统：
    // "以后如果发生任何 WiFi 相关的事件，请调用我这个 wifi_event_handler 函数。"
    // WiFi断开 → WiFi驱动检测到 → 发送事件 → 事件循环 → 调用我们的handler → 执行esp_wifi_connect()
    //- 不是我们主动轮询 ：我们没有写 while(1) {检查是否断开;}
    //- 是系统通知我们 ：WiFi 状态变化时，系统会"主动通知"我们的回调函数
    //- 事件驱动模式 ：这是嵌入式系统中非常高效的设计，不需要一直占用 CPU 检查状态
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));

    // WiFi 配置
    wifi_config_t wifi_config = {0};
    strlcpy((char *)wifi_config.sta.ssid, p->ssid, sizeof(wifi_config.sta.ssid));
    strlcpy((char *)wifi_config.sta.password, p->password, sizeof(wifi_config.sta.password));

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_connect());

    // 守护任务循环 - 保持任务运行以处理连接状态
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// 检查 WiFi 连接状态
bool wifi_is_connected(void)
{
    return s_wifi_connected;
}