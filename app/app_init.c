/*******************************************************************************
File name   : app_init.c
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-10-29
Description : 初始化管理模块实现文件
History     : 

*******************************************************************************/

/** @file app_init.c
 *  @brief 初始化管理模块实现
 *  @details 实现系统初始化阶段的管理功能，按照优先级执行已注册的初始化函数
 */

/* Includes ------------------------------------------------------------------*/
#define DEBUG_LEVEL DBG_LV_DEBUG
#define DEBUG_TAG   "init"

#include "app_config.h"
#if USE_APP_INIT
#include "app_init.h"

/* Private variables ---------------------------------------------------------*/
/** @brief 初始化函数注册段定义 */
SECTION_DEF(APP_INIT_SECTION, app_init_register_t);

/* Private functions ---------------------------------------------------------*/
/** @brief 获取初始化级别字符串
 *  @param level 初始化级别
 *  @return 初始化级别对应的字符串
 */
app_used static char *get_init_level_string(uint8_t level)
{
    switch (level) {
        CASE_TO_NAME(APP_INIT_LEVEL_BOARD);
        CASE_TO_NAME(APP_INIT_LEVEL_PREV);
        CASE_TO_NAME(APP_INIT_LEVEL_DEVICE);
        CASE_TO_NAME(APP_INIT_LEVEL_COMPONENT);
        CASE_TO_NAME(APP_INIT_LEVEL_APPLICATION);
        default: break;
    }
    return "INIT_NONE";
}

/* Exported functions --------------------------------------------------------*/
/** @brief 执行所有已注册的初始化函数
 *  @details 按照初始化级别从低到高依次执行所有已注册的初始化函数：
 *           1. 遍历所有初始化级别
 *           2. 对每个级别，查找并执行对应的初始化函数
 *           3. 支持调试模式下的执行日志输出
 *  @note 该函数需要在系统启动时调用
 */
void app_init_process(void)
{
    const uint32_t *p_start = (const uint32_t*)SECTION_START_ADDR(APP_INIT_SECTION);
    const uint32_t *p_end = (const uint32_t*)SECTION_END_ADDR(APP_INIT_SECTION);
    const uint32_t *p_section_start = p_start;
    app_init_register_t *p_handle;
    for (uint8_t i=0; i<APP_INIT_LEVEL_MAX; i++) {
        p_start = p_section_start;
        while (p_start < p_end) {
            p_handle = (app_init_register_t*)p_start;
            if (p_handle && p_handle->handle && p_handle->level == i) {
                #if APP_INIT_DEBUG
                print_debug("execution: %s, level:%s", p_handle->name, get_init_level_string(p_handle->level));
                #endif
                p_handle->handle();
            }
            p_start = section_next_item(p_start, p_end, sizeof(app_init_register_t));
        }
    }
}

#endif /* USE_APP_INIT */
/************************ END OF FILE *****************************************/
