/*******************************************************************************
File name   : app_timer.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-11-10
Description : 软件定时器模块头文件，提供定时器的注册、管理和执行功能
              1.当前定时器不考虑定时器的模式是否为单次还是重复
              2.目前timer比较单一，使用量不高，没有做功能优化
History     : 
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_TIMER_H__
#define __APP_TIMER_H__

#ifdef __cplusplus
  extern "C" {
#endif
/* ---------------------------------------------------------------------------*/
#include "app_compiler.h"
#include "app_config.h"

/** @defgroup APP_TIMER Software Timer Module
 * @brief 软件定时器模块
 * @{
 */

/** @brief Timer功能使能开关
 *  @details 当设置为1时启用定时器功能，设置为0时禁用定时器功能
 */
#ifndef USE_APP_TIMER
#define USE_APP_TIMER                   0
#endif

/** @brief 定时器最小时间片
 *  @details 定义定时器的最小时间精度，单位为毫秒
 *           所有定时器的时间值都将是此值的整数倍
 */
#ifndef APP_TIMER_MIN_SLICE
#define APP_TIMER_MIN_SLICE             (10)
#endif

/* Includes ------------------------------------------------------------------*/
#if USE_APP_TIMER
#include "app_section.h"
#include "app_common.h"

/* Exported define -----------------------------------------------------------*/

/** @brief 定时器是否使用硬件模式，不影响默认的软件定时器模式
 *  @details 当设置为1时启用硬件模式，需要在外部定时器中断中调用
 */
#ifndef APP_HARD_TIMER_ENABLE
#define APP_HARD_TIMER_ENABLE           0
#endif

/** @brief 定时器延迟扩展功能开关
 *  @details 当设置为1时，app_timer_set_delay，单次修改定时器周期，
 *           触发后自动恢复原始周期；关闭时不提供 app_timer_set_delay 接口
 */
#ifndef APP_TIMER_DELAY_EXTEND
#define APP_TIMER_DELAY_EXTEND          0
#endif


/** @brief 定时器调试开关
 *  @details 当设置为1时启用调试功能，可以显示定时器名称
 */
#ifndef APP_TIMER_DEBUG
#define APP_TIMER_DEBUG                 0
#endif

/** @brief 定时器性能调试开关
 *  @details 当设置为1时启用性能监控功能
 */
#ifndef APP_TIMER_PERF_DEBUG
#define APP_TIMER_PERF_DEBUG            1
#endif

/** @brief 性能调试阈值
 *  @details 当定时器回调函数执行时间超过此阈值（单位：微秒）时，会打印调试信息
 */
#ifndef APP_TIMER_PERF_DEBUG_THRESHOLD
#define APP_TIMER_PERF_DEBUG_THRESHOLD  (10)
#endif

/* Exported macro ------------------------------------------------------------*/

/** @brief 定时器注册段名
 *  @details 用于注册定时器的内存段名称
 */
#define APP_TIMER_SECTION               app_timer_section
#if     APP_HARD_TIMER_ENABLE
#define APP_HARD_TIMER_SECTION               app_hard_timer_section
#endif

/** @brief 注册定时器宏
 *  @param ihandle 定时器回调函数句柄
 *  @param iparameter 传递给回调函数的参数
 *  @param itimeout 超时时间（单位：毫秒）
 *  @param irepeated 是否重复执行（0：不重复，1：重复）
 *  @param ifirst_time 首次执行时间（单位：毫秒）
 * 
 *  @warning 1.当前模式不考虑定时器的模式是否为单次还是重复
 *           2.所有时间参数必须是APP_TIMER_MIN_SLICE的整数倍
 */
#if APP_TIMER_DEBUG
#define APP_TIMER_SECTION_REGISTER(section, ihandle, iparameter, itimeout, irepeated, ifirst_time)   \
    static app_timer_t CONCAT_2(ihandle, _val) = { \
        .handle     = ihandle,          \
        .parameter  = iparameter,       \
        .timeout    = itimeout / APP_TIMER_MIN_SLICE,       \
        .repeated   = irepeated,        \
        .have_first = (ifirst_time / APP_TIMER_MIN_SLICE ? 1 : 0),              \
        .last_time  = ifirst_time / APP_TIMER_MIN_SLICE,    \
    };                                  \
    SECTION_ITEM_REGISTER(section, static const app_timer_register_t CONCAT_2(ihandle, _var)) = { \
        .name       = #ihandle,         \
        .p_timer    = &CONCAT_2(ihandle, _val)              \
    }
#else
#define APP_TIMER_SECTION_REGISTER(section, ihandle, iparameter, itimeout, irepeated, ifirst_time)   \
    static app_timer_t CONCAT_2(ihandle, _val) = { \
        .handle     = ihandle,          \
        .parameter  = iparameter,       \
        .timeout    = itimeout / APP_TIMER_MIN_SLICE,       \
        .repeated   = irepeated,        \
        .have_first = (ifirst_time / APP_TIMER_MIN_SLICE ? 1 : 0),              \
        .last_time  = ifirst_time / APP_TIMER_MIN_SLICE,    \
    };                                  \
    SECTION_ITEM_REGISTER(section, static const app_timer_register_t CONCAT_2(ihandle, _var)) = { \
        .p_timer    = &CONCAT_2(ihandle, _val)              \
    }
#endif

/**
 * @brief 注册一个应用到软件定时器
 */
#define APP_TIMER_REGISTER(ihandle, iparameter, itimeout, irepeated, ifirst_time)       \
        APP_TIMER_SECTION_REGISTER(APP_TIMER_SECTION, ihandle, iparameter, itimeout, irepeated, ifirst_time)

/**
 * @brief 注册一个应用到硬件定时器
 * @note 当未开启 APP_HARD_TIMER_ENABLE 时，此宏会重定向到 APP_TIMER_REGISTER
 */
#if     APP_HARD_TIMER_ENABLE
#define APP_HARD_TIMER_REGISTER(ihandle, iparameter, itimeout, irepeated, ifirst_time)  \
        APP_TIMER_SECTION_REGISTER(APP_HARD_TIMER_SECTION, ihandle, iparameter, itimeout, irepeated, ifirst_time)
#else
#define APP_HARD_TIMER_REGISTER(ihandle, iparameter, itimeout, irepeated, ifirst_time)  \
        APP_TIMER_REGISTER(ihandle, iparameter, itimeout, irepeated, ifirst_time)
#endif

/* Exported typedef ----------------------------------------------------------*/

/** @brief 定时器回调函数类型定义
 *  @param parameter 传递给回调函数的参数
 *  @return 返回执行结果
 */
typedef uint32_t (*timer_exec_handle_t)(void *parameter);

/** @brief 定时器控制结构体
 *  @details 用于存储单个定时器的所有控制信息
 */
typedef struct
{
    const timer_exec_handle_t handle;   /*!< 定时器回调函数 */
    void * const *parameter;            /*!< 传递给回调函数的参数 */
    uint32_t timeout;                   /*!< 超时时间（以APP_TIMER_MIN_SLICE为单位） */
#if APP_TIMER_DELAY_EXTEND
    uint32_t restore_timeout;           /*!< 延迟扩展时保存的原始周期，触发后恢复 */
#endif
    uint32_t repeated:1;                /*!< 是否重复执行标志位 */
    uint32_t have_first:1;              /*!< 是否有首次启动时间标志位 */
    uint32_t last_time:30;              /*!< 上次运行时间（以APP_TIMER_MIN_SLICE为单位） */
} app_timer_t;

/** @brief 定时器注册结构体
 *  @details 用于存储定时器的注册信息
 */
typedef struct
{
    #if APP_TIMER_DEBUG
    const char* name;                   /*!< 定时器名称（仅在调试模式下有效） */
    #endif
    const app_timer_t *p_timer;         /*!< 指向定时器控制结构体的指针 */
} app_timer_register_t;

/* Exported functions --------------------------------------------------------*/

/** @brief 软件定时器处理函数
 *  @param parameter 未使用的参数，为保持接口一致性而保留
 *  @return uint32_t 返回APP_EOK表示处理成功
 *  @details 此函数处理所有已注册的软件定时器
 *  @note 该函数需要在主循环中周期性调用
 */
uint32_t app_timer_process(void *parameter);

#if APP_TIMER_DELAY_EXTEND
/** @brief 设置定时器下次触发的延迟时间，不影响后续周期
 *  @param p_timer  定时器对象指针
 *  @param delay_ms 距下次触发的延迟时间（单位：毫秒），0 表示立即触发
 *  @param reset    true：从当前时刻起计算延迟；false：从上次触发时刻起计算延迟
 *  @note  delay_ms > timeout 时，临时修改周期为 delay_ms，触发后自动恢复原始周期
 */
void app_timer_set_delay(app_timer_t *p_timer, uint32_t delay_ms, bool reset);

/**
 * @brief 设置定时器下次触发的延迟时间，不影响后续周期
 * @param ihandle  注册定时器时使用的回调函数句柄
 * @param delay_ms 距下次触发的延迟时间（毫秒），0 表示立即触发
 * @param reset    true：从当前时刻起计算延迟；false：从上次触发时刻起计算延迟
 */
#define app_timer_delay(ihandle, delay_ms, reset)  app_timer_set_delay(&CONCAT_2(ihandle, _val), delay_ms, reset)
#endif

#if     APP_HARD_TIMER_ENABLE
/**
 * @brief 硬件定时器处理函数
 * @param parameter 未使用的参数，为保持接口一致性而保留
 * @return uint32_t 返回APP_EOK表示处理成功
 * @details 此函数处理所有已注册的硬件定时器
 * @note 该函数需要在定时器中断中周期性调用
 */
uint32_t app_hard_timer_process(void *parameter);
#else
app_inline uint32_t app_hard_timer_process(void *parameter) { return 0; }
#endif

/* ---------------------------------------------------------------------------*/
#else
#define APP_TIMER_REGISTER(ihandle, iparameter, itimeout, irepeated, ifirst_time)
#define APP_HARD_TIMER_REGISTER(ihandle, iparameter, itimeout, irepeated, ifirst_time)
app_inline uint32_t app_timer_process(void *parameter) { return 0; }
app_inline uint32_t app_hard_timer_process(void *parameter) { return 0; }
#endif
/* ---------------------------------------------------------------------------*/

/** @} */ // end of APP_TIMER

#ifdef __cplusplus
}
#endif

#endif /* __APP_TIMER_H__ */

/************************ END OF FILE *****************************************/
