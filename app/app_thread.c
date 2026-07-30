/*******************************************************************************
File name   : app_thread.c
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-11-10
Description : 线程管理模块实现文件，提供线程注册、调度和执行功能的具体实现
History     : 

*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#define DEBUG_LEVEL DBG_LV_DEBUG
#define DEBUG_TAG   "thread"

#include "app_config.h"
#if USE_APP_THREAD
#include "app_thread.h"

/* Private variables ---------------------------------------------------------*/
/** @brief 线程函数注册段定义
 *  @details 使用SECTION_DEF宏定义一个存储线程注册信息的内存段
 */
SECTION_DEF(APP_THREAD_SECTION, app_thread_register_t);

/* Exported functions --------------------------------------------------------*/

/** @brief 线程处理函数
 *  @details 此函数执行所有已注册的线程函数，按级别从0到APP_THREAD_LEVEL_MAX依次执行
 *           同一级别的线程函数按照注册顺序执行
 *  
 *  @note 此函数需要在主循环中周期性调用
 *  @warning 线程函数的执行时间不应过长，以免影响其他线程的执行
 */
void app_thread_process(void)
{
    const uint32_t *p_start = (const uint32_t*)SECTION_START_ADDR(APP_THREAD_SECTION);
    const uint32_t *p_end = (const uint32_t*)SECTION_END_ADDR(APP_THREAD_SECTION);
    const uint32_t *p_section_start = p_start;
    app_thread_register_t *p_handle;
    for (uint8_t i=0; i<=APP_THREAD_LEVEL_MAX; i++) {
        p_start = p_section_start;
        while (p_start < p_end) {
            p_handle = (app_thread_register_t*)p_start;
            if (p_handle && p_handle->handle && p_handle->level == i) {
                #if APP_THREAD_DEBUG
                print_debug("execution: %s, level:%d", p_handle->name, p_handle->level);
                #endif
                #if APP_THREAD_PERF_DEBUG
                uint32_t start = app_time_ms_get();
                #endif
                p_handle->handle(*(p_handle->parameter));
                #if APP_THREAD_PERF_DEBUG
                uint32_t end = app_time_ms_get();
                uint32_t diff = app_time_ms_diff(end, start);
                if (diff >= APP_THREAD_PERF_DEBUG_THRESHOLD) {
                    #if APP_THREAD_DEBUG
                    print_debug("name=%s, run time: %dms", p_handle->name, diff);
                    #else
                    print_debug("handle=0x%08x, run time: %dms", p_handle->handle, diff);
                    #endif
                }
                #endif
            }
            p_start = section_next_item(p_start, p_end, sizeof(app_thread_register_t));
        }
    }
}

#endif /* USE_APP_THREAD */
/************************ END OF FILE *****************************************/
