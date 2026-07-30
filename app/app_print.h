/*******************************************************************************
File name   : app_print.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-11-06
Description : 
History     : 25-11-06  新建，支持RTT打印
              25-11-25  添加对串口打印方式的支持
Note        : print_log 仅为了兼容旧版的打印接口，后续不建议使用
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_PRINT_H__
#define __APP_PRINT_H__

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_config.h"

/* Exported define -----------------------------------------------------------*/
/* print debug level */
#define DBG_LV_ERROR                    0
#define DBG_LV_WARN                     1
#define DBG_LV_INFO                     2
#define DBG_LV_DEBUG                    3

/* Exported macro ------------------------------------------------------------*/
#define app_print_enable(en)            (g_print_enable = en)
#define app_print_is_enable()           (g_print_enable == true)

#ifndef DEBUG_TAG
#define DEBUG_TAG                       ""
#endif

#if USE_SEGGER_RTT
    #define kprintf(fmt, ...)           SEGGER_RTT_printf(RTT_DBG_PORT, fmt, ##__VA_ARGS__)

    #if defined(DEBUG_LEVEL)&&(DEBUG_LEVEL >= DBG_LV_DEBUG)
    #define print_debug(fmt, ...)       do { \
        if(g_print_enable)              SEGGER_RTT_printf(RTT_DBG_PORT, "%s%s(%d):"fmt"\r\n", app_print_time_get(), DEBUG_TAG, __LINE__, ##__VA_ARGS__); \
    }while(0)

    #define print_log(fmt, ...)         do { \
        if(g_print_enable)              SEGGER_RTT_printf(RTT_DBG_PORT, fmt, ##__VA_ARGS__); \
    }while(0)

    #define print_hex(tag, buff, size)  do { \
        if(g_print_enable)              app_hex_print(tag, buff, size); \
    }while(0)
    #endif

    #if defined(DEBUG_LEVEL)&&(DEBUG_LEVEL >= DBG_LV_INFO)
    #define print_info(fmt, ...)        do {   \
        if(g_print_enable)              SEGGER_RTT_printf(RTT_DBG_PORT, "\033[1;32m%s%s(%d):"fmt"\r\n\033[0m", app_print_time_get(), DEBUG_TAG, __LINE__, ##__VA_ARGS__); \
    }while(0)
    #endif

    #if defined(DEBUG_LEVEL)&&(DEBUG_LEVEL >= DBG_LV_WARN)
    #define print_warn(fmt, ...)        do {   \
        if(g_print_enable)              SEGGER_RTT_printf(RTT_DBG_PORT, "\033[1;33m%s%s(%d):"fmt"\r\n\033[0m", app_print_time_get(), DEBUG_TAG, __LINE__, ##__VA_ARGS__); \
    }while(0)
    #endif

    #if defined(DEBUG_LEVEL)&&(DEBUG_LEVEL >= DBG_LV_ERROR)
    #define print_error(fmt, ...)       do {   \
        if(g_print_enable)              SEGGER_RTT_printf(RTT_DBG_PORT, "\033[1;31m%s%s(%d):"fmt"\r\n\033[0m", app_print_time_get(), DEBUG_TAG, __LINE__, ##__VA_ARGS__); \
    }while(0)
    #endif
#elif USE_UART_PRINTF
    #if USE_RT_THREAD_PRINTF
    void rt_hw_console_output(const char *str); /* 需要重新实现该函数 */
    void rt_kprintf(const char *fmt, ...);
    #define kprintf                      rt_kprintf
    #else
    #define kprintf                      printf
    #endif

    #if defined(DEBUG_LEVEL)&&(DEBUG_LEVEL >= DBG_LV_DEBUG)
    #define print_debug(fmt, ...)       do { \
        if(g_print_enable)              kprintf("%s %s(%d):"fmt"\r\n", app_print_time_get(), DEBUG_TAG, __LINE__, ##__VA_ARGS__); \
    }while(0)

    #define print_log(fmt, ...)         do { \
        if(g_print_enable)              kprintf(fmt, ##__VA_ARGS__); \
    }while(0)

    #define print_hex(tag, buff, size)  do { \
        if(g_print_enable)              app_hex_print(tag, buff, size); \
    }while(0)
    #endif

    #if defined(DEBUG_LEVEL)&&(DEBUG_LEVEL >= DBG_LV_INFO)
    #define print_info(fmt, ...)        do {   \
        if(g_print_enable)              kprintf("%s %s(%d):"fmt"\r\n", app_print_time_get(), DEBUG_TAG, __LINE__, ##__VA_ARGS__); \
    }while(0)
    #endif

    #if defined(DEBUG_LEVEL)&&(DEBUG_LEVEL >= DBG_LV_WARN)
    #define print_warn(fmt, ...)        do {   \
        if(g_print_enable)              kprintf("[warn] %s %s(%d):"fmt"\r\n", app_print_time_get(), DEBUG_TAG, __LINE__, ##__VA_ARGS__); \
    }while(0)
    #endif

    #if defined(DEBUG_LEVEL)&&(DEBUG_LEVEL >= DBG_LV_ERROR)
    #define print_error(fmt, ...)       do {   \
        if(g_print_enable)              kprintf("[error] %s %s(%d):"fmt"\r\n", app_print_time_get(), DEBUG_TAG, __LINE__, ##__VA_ARGS__); \
    }while(0)
    #endif
#endif

#ifndef kprintf
#define kprintf(fmt, ...)
#endif
#ifndef print_log
#define print_log(fmt, ...)
#endif
#ifndef print_debug
#define print_debug(fmt,...)
#endif
#ifndef print_info
#define print_info(fmt,...)
#endif
#ifndef print_warn
#define print_warn(fmt,...)
#endif
#ifndef print_error
#define print_error(fmt,...)
#endif
#ifndef print_hex
#define print_hex(tag, buff, size)
#endif

/* Exported variables --------------------------------------------------------*/
extern bool g_print_enable;

/* Exported functions --------------------------------------------------------*/
// @brief 获取当前时间
const char* app_print_time_get(void);
void app_hex_print(uint8_t *name, uint8_t *buff, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* __APP_PRINT_H__ */

/************************ END OF FILE *****************************************/
