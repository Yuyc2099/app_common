/*******************************************************************************
File name   : app_thread.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-11-10
Description : 
History     : 
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_THREAD_H__
#define __APP_THREAD_H__

#ifdef __cplusplus
  extern "C" {
#endif
/* ---------------------------------------------------------------------------*/
#include "app_compiler.h"
#include "app_config.h"

/** @defgroup APP_THREAD Thread Management Module
 * @brief 线程管理模块
 * @{
 */

/** @brief Thread功能使能开关
 *  @details 当设置为1时启用线程功能，设置为0时禁用线程功能
 */
#ifndef USE_APP_THREAD
#define USE_APP_THREAD                  0
#endif

/** @brief 最大线程级别数
 *  @details 定义线程的最大级别，不建议设置太大，建议最大值为16
 */
#ifndef APP_THREAD_LEVEL_MAX
#define APP_THREAD_LEVEL_MAX            8
#endif

/* Includes ------------------------------------------------------------------*/
#if USE_APP_THREAD
#include "app_section.h"
#include "app_common.h"

/* Exported define -----------------------------------------------------------*/

/** @brief 线程调试开关
 *  @details 当设置为1时启用调试功能，可以显示线程名称
 */
#ifndef APP_THREAD_DEBUG
#define APP_THREAD_DEBUG                0
#endif

/** @brief 线程性能调试开关
 *  @details 当设置为1时启用性能监控功能
 */
#ifndef APP_THREAD_PERF_DEBUG
#define APP_THREAD_PERF_DEBUG           1
#endif

/** @brief 性能调试阈值
 *  @details 当线程执行时间超过此阈值（单位：微秒）时，会打印调试信息
 */
#ifndef APP_THREAD_PERF_DEBUG_THRESHOLD
#define APP_THREAD_PERF_DEBUG_THRESHOLD (100)
#endif

#define APP_THREAD_LOW_LEVEL            (APP_THREAD_LEVEL_MAX - 1)
#define APP_THREAD_MIDDLE_LEVEL         (APP_THREAD_LEVEL_MAX / 2)
#define APP_THREAD_HIGH_LEVEL           (0)

/** @brief 函数注册段名
 *  @details 用于注册线程函数的内存段名称
 */
#define APP_THREAD_SECTION              app_thread_section


/* Exported macro ------------------------------------------------------------*/

/** @brief 注册线程函数宏
 *  @param ihandle 线程函数句柄
 *  @param iparameter 传递给线程函数的参数
 *  @param ilevel 线程的执行级别（0-APP_THREAD_LEVEL_MAX）, 级别越低优先级越高
 *  @details 此宏用于将线程函数注册到系统中，并指定其执行级别
 */
#if APP_THREAD_DEBUG
#define APP_THREAD_REGISTER(ihandle, iparameter, ilevel)    \
    SECTION_ITEM_REGISTER(APP_THREAD_SECTION, static const app_thread_register_t CONCAT_2(ihandle, _var)) = { \
        .name       = #ihandle,         \
        .handle     = ihandle,          \
        .parameter  = iparameter,       \
        .level      = (ilevel > APP_THREAD_LEVEL_MAX ? APP_THREAD_LEVEL_MAX : ilevel), \
    }
#else
#define APP_THREAD_REGISTER(ihandle, iparameter, ilevel)    \
    SECTION_ITEM_REGISTER(APP_THREAD_SECTION, static const app_thread_register_t CONCAT_2(ihandle, _var)) = { \
        .handle     = ihandle,          \
        .parameter  = iparameter,       \
        .level      = (ilevel > APP_THREAD_LEVEL_MAX ? APP_THREAD_LEVEL_MAX : ilevel), \
    }
#endif

/* Exported typedef ----------------------------------------------------------*/

/** @brief 线程执行函数类型定义
 *  @param parameter 传递给线程函数的参数
 *  @return 返回执行结果
 */
typedef uint32_t (*thread_exec_handle_t)(void *parameter);

/** @brief 线程注册结构体
 *  @details 用于存储线程的相关信息，包括函数指针、参数和执行级别
 */
typedef struct
{
    #if APP_THREAD_DEBUG
    const char* name;                   /*!< 线程函数名称（仅在调试模式下有效） */
    #endif
    const thread_exec_handle_t handle;  /*!< 线程执行函数指针 */
    void * const *parameter;            /*!< 传递给线程函数的参数 */
    const uint8_t level;                /*!< 线程执行级别 */
} app_thread_register_t;

/* Exported functions --------------------------------------------------------*/

/** @brief 线程处理函数
 *  @details 此函数需要在主循环中周期性调用，用于执行所有注册的线程函数
 *  @note 线程函数将按照注册时指定的级别顺序执行
 */
void app_thread_process(void);

/* ---------------------------------------------------------------------------*/
#else
#define APP_THREAD_REGISTER(ihandle, iparameter, ilevel)
app_inline void app_thread_process(void) {}
#endif
/* ---------------------------------------------------------------------------*/

/** @} */ // end of APP_THREAD

#ifdef __cplusplus
}
#endif

#endif /* __APP_THREAD_H__ */

/************************ END OF FILE *****************************************/
