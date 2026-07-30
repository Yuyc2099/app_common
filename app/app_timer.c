/*******************************************************************************
File name   : app_timer.c
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-11-10
Description : 定时器模块实现文件，提供软件定时器的注册、管理和执行功能
History     : 

*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#define DEBUG_LEVEL DBG_LV_DEBUG
#define DEBUG_TAG   "timer"

#include "app_config.h"
#if USE_APP_TIMER
#include "app_timer.h"

/* Private variables ---------------------------------------------------------*/

/** @brief 定时器注册段定义
 *  @details 使用SECTION_DEF宏定义一个存储定时器注册信息的内存段
 */
SECTION_DEF(APP_TIMER_SECTION, app_timer_register_t);

#if APP_HARD_TIMER_ENABLE
SECTION_DEF(APP_HARD_TIMER_SECTION, app_timer_register_t);
#endif

/* Private functions ---------------------------------------------------------*/

/** @brief 检查定时器是否超时
 *  @param p_timer 定时器对象指针
 *  @param tick 当前系统时钟计数值
 *  @return true 定时器已超时
 *  @return false 定时器未超时
 *  @details 此函数用于检查单个定时器是否达到超时条件
 *           - 如果是首次执行，使用last_time作为超时时间
 *           - 后续执行使用timeout作为超时间隔
 *           - 超时后会自动更新last_time为当前时间
 */
static bool is_timerout(app_timer_t* p_timer, uint32_t time_slice)
{
    uint32_t last_time = p_timer->last_time;
    uint32_t timeout   = p_timer->timeout;
    if (p_timer->have_first) {
        last_time = 0;
        timeout   = p_timer->last_time;
    }
    if (app_time_ms_diff(time_slice, last_time) >= timeout) {
        p_timer->last_time = time_slice; // todo: 需要优化下，不然后存在累计的定时器误差
        if (p_timer->have_first) {
            p_timer->have_first = false;
        }
#if APP_TIMER_DELAY_EXTEND
        if (p_timer->restore_timeout) {
            p_timer->timeout = p_timer->restore_timeout;
            p_timer->restore_timeout = 0;
        }
#endif
        return true;
    }
    return false;
}

/* Exported functions --------------------------------------------------------*/

#if APP_TIMER_DELAY_EXTEND
/** @brief 设置定时器下次触发的延迟时间，不影响后续周期
 *  @param p_timer  定时器对象指针
 *  @param delay_ms 距下次触发的延迟时间，单位毫秒，0 表示立即触发
 *  @param reset    true：从当前时刻起计算延迟；false：从上次触发时刻起计算延迟
 *  @note  delay_ms > timeout 时，临时修改周期为 delay_ms，触发后自动恢复原始周期
 */
void app_timer_set_delay(app_timer_t *p_timer, uint32_t delay_ms, bool reset)
{
    uint32_t delay_slice = delay_ms / APP_TIMER_MIN_SLICE;
    // 只在无待恢复值时才保存原始周期，避免重复调用时将已修改的 timeout 当成原始值保存
    if (p_timer->restore_timeout == 0) {
        p_timer->restore_timeout = p_timer->timeout;
    }
    p_timer->timeout = delay_slice ? delay_slice : 1;
    if (reset) {
        p_timer->last_time = app_time_ms_get() / APP_TIMER_MIN_SLICE;
    }
}
#endif

/** @brief 软件定时器处理函数
 *  @param parameter 未使用的参数，为保持接口一致性而保留
 *  @return uint32_t 返回APP_EOK表示处理成功
 * 
 *  @note 该函数需要在主循环中周期性调用
 *  @warning 回调函数的执行时间不应过长，以免影响其他定时器的处理
 */
uint32_t app_timer_process(void *parameter)
{
    static uint32_t last_time_slice = 0;
    uint32_t time_slice = app_time_ms_get() / APP_TIMER_MIN_SLICE;
    if (time_slice == last_time_slice) {
        return APP_EOK;
    }

    last_time_slice = time_slice;
    const uint32_t *p_start = (const uint32_t*)SECTION_START_ADDR(APP_TIMER_SECTION);
    const uint32_t *p_end = (const uint32_t*)SECTION_END_ADDR(APP_TIMER_SECTION);
    app_timer_t *p_handle;
    app_timer_register_t *p_register;
    while (p_start < p_end) {
        p_register = (app_timer_register_t*)p_start;
        p_handle = (app_timer_t *)(p_register->p_timer);
        if (p_handle && p_handle->handle && is_timerout(p_handle, time_slice)) {
            #if APP_TIMER_DEBUG
            print_debug("execution: %s", p_register->name);
            #endif
            #if APP_TIMER_PERF_DEBUG
            uint32_t start = app_time_ms_get();
            #endif
            p_handle->handle(*(p_handle->parameter));
            #if APP_TIMER_PERF_DEBUG
            uint32_t end = app_time_ms_get();
            uint32_t diff = app_time_ms_diff(end, start);
            if (diff >= APP_TIMER_PERF_DEBUG_THRESHOLD) {
                #if APP_TIMER_DEBUG
                print_debug("name=%s, run time: %dms", p_register->name, diff);
                #else
                print_debug("handle=0x%08x, run time: %dms", p_handle->handle, diff);
                #endif
            }
            #endif
        }
        p_start = section_next_item(p_start, p_end, sizeof(app_timer_register_t));
    }
    return APP_EOK;
}

APP_THREAD_REGISTER(app_timer_process, NULL, APP_THREAD_LOW_LEVEL);

#if     APP_HARD_TIMER_ENABLE
/**
 * @brief 硬件定时器处理函数
 * @param parameter 未使用的参数，为保持接口一致性而保留
 * @return uint32_t 返回APP_EOK表示处理成功
 * 
 *  @note 该函数需要在定时器中断中周期性调用
 *  @warning 回调函数的执行时间不应过长，以免影响其他定时器的处理
 */
uint32_t app_hard_timer_process(void *parameter)
{
    uint32_t time_slice = app_time_ms_get() / APP_TIMER_MIN_SLICE;
    const uint32_t *p_start = (const uint32_t*)SECTION_START_ADDR(APP_HARD_TIMER_SECTION);
    const uint32_t *p_end = (const uint32_t*)SECTION_END_ADDR(APP_HARD_TIMER_SECTION);
    app_timer_t *p_handle;
    app_timer_register_t *p_register;
    while (p_start < p_end) {
        p_register = (app_timer_register_t*)p_start;
        p_handle = (app_timer_t *)(p_register->p_timer);
        if (p_handle && p_handle->handle && is_timerout(p_handle, time_slice)) {
            #if APP_TIMER_DEBUG
            print_debug("execution: %s", p_register->name);
            #endif
            #if APP_TIMER_PERF_DEBUG
            uint32_t start = app_time_ms_get();
            #endif
            p_handle->handle(*(p_handle->parameter));
            #if APP_TIMER_PERF_DEBUG
            uint32_t end = app_time_ms_get();
            uint32_t diff = app_time_ms_diff(end, start);
            if (diff >= APP_TIMER_PERF_DEBUG_THRESHOLD) {
                #if APP_TIMER_DEBUG
                print_debug("name=%s, run time: %dms", p_register->name, diff);
                #else
                print_debug("handle=0x%08x, run time: %dms", p_handle->handle, diff);
                #endif
            }
            #endif
        }
        p_start = section_next_item(p_start, p_end, sizeof(app_timer_register_t));
    }
    return APP_EOK;
}
#endif /* APP_HARD_TIMER_ENABLE */
#endif /* USE_APP_TIMER */
/************************ END OF FILE *****************************************/
