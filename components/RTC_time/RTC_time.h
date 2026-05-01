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
#include "esp_err.h"


    esp_err_t RTC_init(void);

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
