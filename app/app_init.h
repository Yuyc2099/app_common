/*******************************************************************************
File name   : app_init_section.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-10-29
Description : 初始化管理模块头文件
History     : 
*******************************************************************************/

/** @file app_init_section.h
 *  @brief 初始化管理模块
 *  @details 提供系统初始化阶段的划分和管理功能，支持不同优先级的初始化函数注册
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_INIT_H__
#define __APP_INIT_H__

#ifdef __cplusplus
  extern "C" {
#endif

/* ---------------------------------------------------------------------------*/
#include "app_compiler.h"
#include "app_config.h"

/** @brief 初始化功能使能开关 */
#ifndef USE_APP_INIT
#define USE_APP_INIT                    0
#endif

/* Includes ------------------------------------------------------------------*/
#if USE_APP_INIT
#include "app_section.h"
#include "app_common.h"

/* Exported define -----------------------------------------------------------*/
/** @brief 初始化调试功能使能开关 */
#define APP_INIT_DEBUG                  0

/** @brief 初始化函数注册段名 */
#define APP_INIT_SECTION                app_init_section

/* Exported macro ------------------------------------------------------------*/
/** @brief 注册初始化函数宏
 *  @param ihandle 初始化函数指针
 *  @param ilevel 初始化级别
 */
#if APP_INIT_DEBUG
#define APP_INIT_REGISTER(ihandle, ilevel)      \
    SECTION_ITEM_REGISTER(APP_INIT_SECTION, static const app_init_register_t CONCAT_2(ihandle, _var)) = { \
        .name       = #ihandle,         \
        .handle     = (init_exec_handle_t)ihandle,          \
        .level      = ilevel,           \
    }
#else
#define APP_INIT_REGISTER(ihandle, ilevel)      \
    SECTION_ITEM_REGISTER(APP_INIT_SECTION, static const app_init_register_t CONCAT_2(ihandle, _var)) = { \
        .handle     = (init_exec_handle_t)ihandle,          \
        .level      = ilevel,           \
    }
#endif

/** @brief 注册板级初始化函数 */
#define APP_BOARD_INIT(handle)          APP_INIT_REGISTER(handle, APP_INIT_LEVEL_BOARD)
/** @brief 注册预初始化函数(纯软件初始化) */
#define APP_PREV_INIT(handle)           APP_INIT_REGISTER(handle, APP_INIT_LEVEL_PREV)
/** @brief 注册设备初始化函数 */
#define APP_DEVICE_INIT(handle)         APP_INIT_REGISTER(handle, APP_INIT_LEVEL_DEVICE)
/** @brief 注册组件初始化函数(如dfs、lwip等) */
#define APP_COMPONENT_INIT(handle)      APP_INIT_REGISTER(handle, APP_INIT_LEVEL_COMPONENT)
/** @brief 注册应用初始化函数(如rtgui应用等) */
#define APP_APPLICATION_INIT(handle)    APP_INIT_REGISTER(handle, APP_INIT_LEVEL_APPLICATION)

/* Exported typedef ----------------------------------------------------------*/
/** @brief 初始化级别枚举 */
typedef enum 
{
    APP_INIT_LEVEL_BOARD = 0,           /*!< 板级初始化 */
    APP_INIT_LEVEL_PREV,                /*!< 预初始化 */
    APP_INIT_LEVEL_DEVICE,              /*!< 设备初始化 */
    APP_INIT_LEVEL_COMPONENT,           /*!< 组件初始化 */
    APP_INIT_LEVEL_APPLICATION,         /*!< 应用初始化 */
    APP_INIT_LEVEL_MAX,                 /*!< 最大初始化级别 */
} app_init_level_e;

typedef uint32_t (*init_exec_handle_t)(void);

/** @brief 初始化函数注册结构体 */
typedef struct {
    #if APP_INIT_DEBUG
    const char* name;                   /*!< 初始化函数名称 */
    #endif
    const init_exec_handle_t handle;    /*!< 初始化函数指针 */
    const uint8_t level;                /*!< 初始化级别 */
} app_init_register_t;

/* Exported functions --------------------------------------------------------*/
/** @brief 执行所有已注册的初始化函数
 *  @details 按照初始化级别从低到高依次执行所有已注册的初始化函数
 *  @note 该函数需要在系统启动时调用
 */
void app_init_process(void);

/* ---------------------------------------------------------------------------*/
#else /* USE_APP_INIT */
#define APP_BOARD_INIT(handle)
#define APP_PREV_INIT(handle)
#define APP_DEVICE_INIT(handle)
#define APP_COMPONENT_INIT(handle)
#define APP_APPLICATION_INIT(handle)
app_inline void app_init_process(void) {}
#endif /* USE_APP_INIT */
/* ---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __APP_INIT_H__ */

/************************ END OF FILE *****************************************/
