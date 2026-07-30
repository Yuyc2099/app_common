/*******************************************************************************
File name   : app_event_.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2026-03-04
Description : 请详细说明此程序文件完成的主要功能 
History     : 修改记录，每条记录应包括修改日期、修改者及修改内容简述
              1. Date: 
                  Author: 
                  Modification: 

*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_EVENT_H__
#define __APP_EVENT_H__

#ifdef __cplusplus
  extern "C" {
#endif

/* ---------------------------------------------------------------------------*/
#include "app_compiler.h"
#include "app_config.h"
/**
 * @defgroup EVENT 事件总线
 * @{
 */

/** @brief 事件功能使能开关 */
#ifndef USE_APP_EVENT
#define USE_APP_EVENT                   0
#endif
/** @brief 事件功能调试功能使能开关 */
#ifndef APP_EVENT_DEBUG
#define APP_EVENT_DEBUG                 0
#endif

/* Includes ------------------------------------------------------------------*/
#if USE_APP_EVENT
#include "app_common.h"
#endif
/* Exported define -----------------------------------------------------------*/

/**
 * @brief 事件总线配置
 */
#ifndef APP_MAX_EVENT_SUBSCRIBERS
#define APP_MAX_EVENT_SUBSCRIBERS   8   /**< 最大订阅者数量 */
#endif

/* Exported macro ------------------------------------------------------------*/
/**
 * @brief 发送事件并处理 (同步处理，无数据)
 */
#define app_event_notify(evtmod, event_id)  app_event_notify_data(evtmod, event_id, NULL)

/* Exported typedef ----------------------------------------------------------*/

/**
 * @brief 事件处理回调函数类型
 * @param evtmod 模块位掩码，对应 APP_EVTMOD_XXX 宏
 * @param event_id 事件ID (number)
 * @param data 事件数据指针 (void*)
 */
typedef void (*app_event_handler_t)(uint32_t evtmod, uint32_t event_id, const void *data);

/* Private moudle variables --------------------------------------------------*/

/* Private moudle functions --------------------------------------------------*/

/* Private moudle end_flag ---------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
#if USE_APP_EVENT

/**
 * @brief 订阅事件
 * @param handler 事件处理函数
 * @param evtmod_mask 关心的事件主ID位掩码，每个bit对应一个模块位位置，可组合
 * @return APP_EOK 订阅成功, APP_EXXX 订阅失败
 */
uint32_t app_event_subscribe(app_event_handler_t handler, uint32_t evtmod_mask);

/**
 * @brief 取消订阅事件
 * @param handler 事件处理函数
 * @return APP_EOK 取消成功, APP_EXXX 取消失败
 */
uint32_t app_event_unsubscribe(app_event_handler_t handler);

/**
 * @brief 发送事件并处理 (同步处理，携带数据)
 * @param evtmod 模块位掩码，对应 APP_EVTMOD_XXX 宏
 * @param event_id 事件ID
 * @param data 事件数据指针 (void*, 可为NULL)
 */
void app_event_notify_data(uint32_t evtmod, uint32_t event_id, const void *data);

/* ---------------------------------------------------------------------------*/
#else
app_inline uint32_t app_event_subscribe(app_event_handler_t handler, uint32_t evtmod_mask) { return APP_EOK; }
app_inline uint32_t app_event_unsubscribe(app_event_handler_t handler) { return APP_EOK; }
app_inline void app_event_notify_data(uint32_t evtmod_bitpos, uint32_t event_id, const void *data) { }
#endif /* USE_APP_EVENT */
/* ---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __APP_EVENT_H__ */

/************************ END OF FILE *****************************************/
