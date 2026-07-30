/*******************************************************************************
File name   : app_event.c
Author      : Yu You Cheng
Version     : V1.0
Date        : 2026-03-04
Description : 请详细说明此程序文件完成的主要功能
History     : 修改记录，每条记录应包括修改日期、修改者及修改内容简述
              1. Date:
                  Author:
                  Modification:

*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "app_config.h"
#if USE_APP_EVENT
#include "app_event.h"

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/**
 * @brief 事件订阅者结构
 */
typedef struct {
    app_event_handler_t handler;    /**< 事件处理函数 */
    uint32_t evtmod_mask;           /**< 关心的事件主ID位掩码，每个bit对应一个模块 */
} app_event_subscriber_t;

/**
 * @brief 事件总线结构
 */
typedef struct {
    uint32_t subscriber_count;            /**< 当前订阅者数量 */
    app_event_subscriber_t subscribers[APP_MAX_EVENT_SUBSCRIBERS];  /**< 订阅者列表 */
} app_event_t;

/* Private variables ---------------------------------------------------------*/
/**
 * @brief 全局事件总线实例 (单例)
 */
static app_event_t g_app_event = {0};

/* Private moudle variables --------------------------------------------------*/

/* Function declaration ------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
 * @brief 检查事件模块掩码是否匹配订阅掩码
 */
static bool app_event_module_match(uint32_t evtmod, uint32_t evtmod_mask)
{
    return (evtmod_mask & evtmod);
}
/* Private moudle functions --------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
 * @brief 订阅事件
 */
uint32_t app_event_subscribe(app_event_handler_t handler, uint32_t evtmod_mask)
{
    if (handler == NULL) {
        return APP_ERROR;
    }

    /* 检查是否已订阅过，更新掩码即可 */
    for (uint8_t i = 0; i < g_app_event.subscriber_count; i++) {
        if (g_app_event.subscribers[i].handler == handler) {
            g_app_event.subscribers[i].evtmod_mask = evtmod_mask;
            return APP_EOK;
        }
    }

    /* 检查订阅者是否已满 */
    if (g_app_event.subscriber_count >= APP_MAX_EVENT_SUBSCRIBERS) {
        return APP_EFULL;
    }

    /* 添加新订阅者 */
    g_app_event.subscribers[g_app_event.subscriber_count].handler = handler;
    g_app_event.subscribers[g_app_event.subscriber_count].evtmod_mask = evtmod_mask;
    g_app_event.subscriber_count++;

    return APP_EOK;
}

/**
 * @brief 取消订阅事件
 * @note  用末尾元素填补当前槽位，subscriber_count减少，槽位立即可被新handler复用
 */
uint32_t app_event_unsubscribe(app_event_handler_t handler)
{
    if (handler == NULL) {
        return APP_ERROR;
    }

    for (uint8_t i = 0; i < g_app_event.subscriber_count; i++) {
        if (g_app_event.subscribers[i].handler == handler) {
            g_app_event.subscribers[i] = g_app_event.subscribers[g_app_event.subscriber_count - 1];
            memset(&g_app_event.subscribers[g_app_event.subscriber_count - 1], 0, sizeof(app_event_subscriber_t));
            g_app_event.subscriber_count--;
            return APP_EOK;
        }
    }

    return APP_EOK;
}

/**
 * @brief 发送事件并处理 (同步处理)
 * @note  evtmod 是模块位掩码，请使用 APP_EVTMOD_XXX 宏
 *        例如: app_event_notify(APP_EVTMOD_TEMP, ...)
 */
void app_event_notify_data(uint32_t evtmod, uint32_t event_id, const void *data)
{
    #if APP_EVENT_DEBUG
    print_debug("app_event_notify_data, evtmod=0x%08x, event_id=0x%08x", evtmod, event_id);
    #endif
    /* 遍历所有订阅者并立即处理事件 */
    for (uint8_t i = 0; i < g_app_event.subscriber_count; i++) {
        app_event_subscriber_t *subscriber = &g_app_event.subscribers[i];

        /* 检查订阅者关心的事件主ID是否匹配 */
        if (subscriber->handler != NULL &&
            app_event_module_match(evtmod, subscriber->evtmod_mask)) {
            /* 调用事件处理函数 */
            subscriber->handler(evtmod, event_id, data);
        }
    }
}

#endif /* USE_APP_EVENT */
/************************ END OF FILE *****************************************/
