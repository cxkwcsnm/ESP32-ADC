/**
 * @file RTC_time.c
 * @brief RTC 时间管理组件实现
 *
 * 本组件使用 ESP-IDF 的标准时间 API 实现，
 * 不依赖网络时间同步功能。
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
 * @brief 初始化 RTC 时间组件
 *
 * 该函数会初始化时间系统，并默认设置为中国标准时区 (CST-8)
 *
 * @return esp_err_t 总是返回 ESP_OK
 */
esp_err_t RTC_init(void)
{
    ESP_LOGI(TAG, "Initializing RTC");

    /**
     * 设置时区为中国标准时间 (CST-8)
     * CST 表示中国标准时间，-8 表示比 UTC 晚 8 小时
     */
    setenv("TZ", "CST-8", 1);
    tzset();

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