/**
 * @file RTC_time.c
 * @brief RTC 时间管理组件实现
 *
 * 本组件使用 ESP-IDF 的标准时间 API 和 SNTP API 实现，
 * 支持网络时间同步功能。
 */

#include "RTC_time.h"

/**
 * @brief 日志标签
 */
static const char *TAG = "RTC_time";

/**
 * @brief 时间字符串缓冲区
 *
 * 用于存储格式化后的时间字符串
 */
static char time_buf[64];

/**
 * @brief 日期字符串缓冲区
 *
 * 用于存储格式化后的日期字符串
 */
static char date_buf[16];

/**
 * @brief 时分字符串缓冲区
 *
 * 用于存储格式化后的时分字符串
 */
static char hhmm_buf[16];

/**
 * @brief SNTP 同步完成标志
 */
static bool s_sntp_sync_done = false;

/**
 * @brief SNTP 同步回调函数
 *
 * @param tv 时间信息
 */
static void sntp_time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "SNTP time synchronization completed");
    s_sntp_sync_done = true;
    
    // 打印同步后的时间
    char strftime_buf[64];
    time_t now = time(NULL);
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
    ESP_LOGI(TAG, "Current time: %s", strftime_buf);
}

/**
 * @brief 初始化 RTC 时间组件（不带 SNTP）
 *
 * 该函数会初始化时间系统，并默认设置为中国标准时区 (CST-8)
 *
 * @return esp_err_t 总是返回 ESP_OK
 */
esp_err_t RTC_init(void)
{
    ESP_LOGI(TAG, "Initializing RTC without SNTP");

    /**
     * 设置时区为中国标准时间 (CST-8)
     * CST 表示中国标准时间，-8 表示比 UTC 晚 8 小时
     */
    setenv("TZ", "CST-8", 1);
    tzset();

    return ESP_OK;
}

/**
 * @brief 初始化 RTC 时间组件并启动 SNTP 同步
 *
 * @param timezone 时区字符串，例如 "CST-8" 表示中国标准时间
 * @return esp_err_t ESP_OK 表示成功
 */
esp_err_t RTC_init_with_sntp(const char *timezone)
{
    ESP_LOGI(TAG, "Initializing RTC with SNTP");

    // 设置时区
    if (timezone != NULL) {
        setenv("TZ", timezone, 1);
    } else {
        setenv("TZ", "CST-8", 1);
    }
    tzset();

    // 设置 SNTP 服务器
    static const char *const ntp_servers[] = {SNTP_SERVERS};
    for (int i = 0; i < sizeof(ntp_servers) / sizeof(ntp_servers[0]); i++) {
        esp_sntp_setservername(i, ntp_servers[i]);
        ESP_LOGI(TAG, "SNTP server %d: %s", i, ntp_servers[i]);
    }

    // 设置同步模式为立即同步
    esp_sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);

    // 设置同步回调函数
    esp_sntp_set_time_sync_notification_cb(sntp_time_sync_notification_cb);

    // 初始化 SNTP
    esp_sntp_init();

    ESP_LOGI(TAG, "SNTP initialized successfully");
    return ESP_OK;
}

/**
 * @brief 检查 SNTP 是否同步完成
 *
 * @return bool true 表示已同步，false 表示未同步
 */
bool RTC_is_sntp_synced(void)
{
    return s_sntp_sync_done;
}

/**
 * @brief 等待 SNTP 同步完成
 *
 * @param timeout_ms 超时时间（毫秒），0 表示无限等待
 * @return esp_err_t ESP_OK 表示同步成功，ESP_TIMEOUT 表示超时
 */
esp_err_t RTC_wait_sntp_sync(uint32_t timeout_ms)
{
    if (s_sntp_sync_done) {
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Waiting for SNTP sync...");

    uint32_t start_ticks = xTaskGetTickCount();
    while (!s_sntp_sync_done) {
        vTaskDelay(pdMS_TO_TICKS(100));
        
        if (timeout_ms > 0) {
            uint32_t elapsed_ticks = xTaskGetTickCount() - start_ticks;
            if (elapsed_ticks >= pdMS_TO_TICKS(timeout_ms)) {
                ESP_LOGE(TAG, "SNTP sync timeout after %lu ms", (unsigned long)timeout_ms);
                return ESP_ERR_TIMEOUT;
            }
        }
    }

    ESP_LOGI(TAG, "SNTP sync completed");
    return ESP_OK;
}

/**
 * @brief 手动触发 SNTP 同步
 *
 * @return esp_err_t ESP_OK 表示成功
 */
esp_err_t RTC_sntp_sync_now(void)
{
    // 重新启动 SNTP 以触发同步
    sntp_restart();
    ESP_LOGI(TAG, "SNTP sync triggered");
    return ESP_OK;
}

/**
 * @brief 设置时区
 *
 * @param timezone 时区字符串，例如 "CST-8" 表示中国标准时间
 */
void RTC_set_timezone(const char *timezone)
{
    if (timezone != NULL)
    {
        /**
         * 设置 TZ 环境变量
         * 第三个参数 1 表示覆盖已存在的 TZ 环境变量
         */
        setenv("TZ", timezone, 1);

        /**
         * 更新 C 库的时区数据
         */
        tzset();

        ESP_LOGI(TAG, "Timezone set to: %s", timezone);
    }
}

/**
 * @brief 获取当前时间结构体
 *
 * @return struct tm 当前时间的 tm 结构体
 */
struct tm RTC_get_time_struct(void)
{
    time_t now;
    struct tm timeinfo;

    /**
     * 获取当前时间（Unix 时间戳）
     */
    time(&now);

    /**
     * 将 Unix 时间戳转换为本地时间的 tm 结构体
     * 使用 localtime_r 保证线程安全
     */
    localtime_r(&now, &timeinfo);

    return timeinfo;
}













/// ==========下面是一些格式化时间字符串的函数，返回静态缓冲区中的字符串 ==========


/**
 * @brief 获取格式化的时间字符串
 *
 * @return char* 格式化后的时间字符串，格式为 "YYYY-MM-DD HH:MM:SS"
 *
 * @note 返回的字符串为静态缓冲区，不可被多个任务同时调用
 */
char *RTC_get_time(void)
{
    time_t now;
    struct tm timeinfo;

    /**
     * 获取当前时间（Unix 时间戳）
     */
    time(&now);

    /**
     * 将 Unix 时间戳转换为本地时间的 tm 结构体
     */
    localtime_r(&now, &timeinfo);

    /**
     * 将 tm 结构体格式化为字符串
     * 格式说明：
     * %Y - 年份（4位）
     * %m - 月份（01-12）
     * %d - 日期（01-31）
     * %H - 小时（00-23，24小时制）
     * %M - 分钟（00-59）
     * %S - 秒（00-59）
     */
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &timeinfo);

    return time_buf;
}

/**
 * @brief 获取年-月-日字符串
 *
 * @return char* 格式化后的日期字符串，格式为 "YYYY-MM-DD"
 *
 * @note 返回的字符串为静态缓冲区，不可被多个任务同时调用
 */
char *RTC_get_YYYY_MM_DD(void)
{
    time_t now;
    struct tm timeinfo;

    time(&now);
    localtime_r(&now, &timeinfo);

    /**
     * 将 tm 结构体格式化为日期字符串
     * 格式说明：
     * %Y - 年份（4位）
     * %m - 月份（01-12）
     * %d - 日期（01-31）
     */
    strftime(date_buf, sizeof(date_buf), "%Y-%m-%d", &timeinfo);

    return date_buf;
}

/**
 * @brief 获取时-分字符串
 *
 * @return char* 格式化后的时间字符串，格式为 "HH:MM"
 *
 * @note 返回的字符串为静态缓冲区，不可被多个任务同时调用
 */
char *RTC_get_HH_MM(void)
{
    time_t now;
    struct tm timeinfo;

    time(&now);
    localtime_r(&now, &timeinfo);

    /**
     * 将 tm 结构体格式化为时分字符串
     * 格式说明：
     * %H - 小时（00-23，24小时制）
     * %M - 分钟（00-59）
     */
    strftime(hhmm_buf, sizeof(hhmm_buf), "%H:%M", &timeinfo);

    return hhmm_buf;
}

char *RTC_get_HH_MM_SS(void)
{
    time_t now;
    struct tm timeinfo;

    time(&now);
    localtime_r(&now, &timeinfo);

    /**
     * 将 tm 结构体格式化为时分秒字符串
     * 格式说明：
     * %H - 小时（00-23，24小时制）
     * %M - 分钟（00-59）
     * %S - 秒（00-59）
     */
    strftime(hhmm_buf, sizeof(hhmm_buf), "%H:%M:%S", &timeinfo);

    return hhmm_buf;
}











