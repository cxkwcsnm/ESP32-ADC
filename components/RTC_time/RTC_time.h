/**
 * @file RTC_time.h
 * @brief RTC 时间管理组件头文件
 *
 * 本组件提供了基于 ESP-IDF 的时间管理功能，
 * 包括 RTC 初始化、时区设置和时间获取等接口。
 */

#ifndef RTC_TIME_H
#define RTC_TIME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * @brief SNTP 服务器地址列表
 */
#define SNTP_SERVERS "pool.ntp.org", "cn.pool.ntp.org", "ntp.aliyun.com"

    /**
     * @brief 初始化 RTC 时间组件（不带 SNTP）
     *
     * @return esp_err_t 总是返回 ESP_OK
     */
    esp_err_t RTC_init(void);

    /**
     * @brief 初始化 RTC 时间组件并启动 SNTP 同步
     *
     * @param timezone 时区字符串，例如 "CST-8" 表示中国标准时间
     * @return esp_err_t ESP_OK 表示成功
     */
    esp_err_t RTC_init_with_sntp(const char *timezone);

    /**
     * @brief 检查 SNTP 是否同步完成
     *
     * @return bool true 表示已同步，false 表示未同步
     */
    bool RTC_is_sntp_synced(void);

    /**
     * @brief 等待 SNTP 同步完成
     *
     * @param timeout_ms 超时时间（毫秒），0 表示无限等待
     * @return esp_err_t ESP_OK 表示同步成功，ESP_TIMEOUT 表示超时
     */
    esp_err_t RTC_wait_sntp_sync(uint32_t timeout_ms);

    /**
     * @brief 手动触发 SNTP 同步
     *
     * @return esp_err_t ESP_OK 表示成功
     */
    esp_err_t RTC_sntp_sync_now(void);

    /**
     * @brief 设置时区
     *
     * @param timezone 时区字符串，例如 "CST-8" 表示中国标准时间
     */
    void RTC_set_timezone(const char *timezone);

    /**
     * @brief 获取当前时间结构体
     *
     * @return struct tm 当前时间的 tm 结构体
     */
    struct tm RTC_get_time_struct(void);





    

    /// ==========下面是一些格式化时间字符串的函数，返回静态缓冲区中的字符串 ==========

    /**
     * @brief 获取格式化的时间字符串
     *
     * @return char* 格式化后的时间字符串，格式为 "YYYY-MM-DD HH:MM:SS"
     *
     * @note 返回的字符串为静态缓冲区，不可被多个任务同时调用
     */
    char *RTC_get_time(void);

    /**
     * @brief 获取年-月-日字符串
     *
     * @return char* 格式化后的日期字符串，格式为 "YYYY-MM-DD"
     *
     * @note 返回的字符串为静态缓冲区，不可被多个任务同时调用
     */
    char *RTC_get_YYYY_MM_DD(void);

    /**
     * @brief 获取时-分字符串
     *
     * @return char* 格式化后的时间字符串，格式为 "HH:MM"
     *
     * @note 返回的字符串为静态缓冲区，不可被多个任务同时调用
     */
    char *RTC_get_HH_MM(void);

    /**
    * @brief 获取时-分-秒字符串
    *
    * @return char* 格式化后的时间字符串，格式为 "HH:MM:SS"
    *
    * @note 返回的字符串为静态缓冲区，不可被多个任务同时调用
    */
    char* RTC_get_HH_MM_SS(void);

#ifdef __cplusplus
}
#endif

#endif
