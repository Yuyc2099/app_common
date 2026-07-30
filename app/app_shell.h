/*******************************************************************************
File name   : app_shell.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-11-05
Description : Shell命令处理模块头文件
History     : 
*******************************************************************************/

/** @file app_shell.h
 *  @brief Shell命令处理模块
 *  @details 提供Shell命令注册和处理功能
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_SHELL_H__
#define __APP_SHELL_H__

#ifdef __cplusplus
  extern "C" {
#endif

/* ---------------------------------------------------------------------------*/
#include "app_compiler.h"
#include "app_config.h"

/** @brief Shell功能使能开关 */
#ifndef USE_APP_SHELL
#define USE_APP_SHELL                   0
#endif

/* Includes ------------------------------------------------------------------*/
#if USE_APP_SHELL
#include "app_section.h"
#include "app_common.h"

/* Exported define -----------------------------------------------------------*/
/** @brief Shell调试功能使能开关 */
#ifndef USE_APP_SHELL
#define APP_SHELL_DEBUG                 0
#endif
/** @brief Shell命令缓冲区最大长度 */
#ifndef APP_SHELL_MAX_BUFF_SIZE
#define APP_SHELL_MAX_BUFF_SIZE         (64)
#endif

/** @brief Shell命令最大参数个数 */
#ifndef APP_SHELL_MAX_TOKENS
#define APP_SHELL_MAX_TOKENS            (8)
#endif

/* Exported macro ------------------------------------------------------------*/
/** @brief Shell命令段名 */
#define APP_SHELL_SECTION               app_shell_section

/** @brief 注册Shell命令宏
 *  @param icmd 命令字符串
 *  @param ihandle 命令处理函数
 */
#if APP_SHELL_DEBUG
#define APP_SHELL_REGISTER(icmd, ihandle)       \
    SECTION_ITEM_REGISTER(APP_SHELL_SECTION, static const app_shell_register_t CONCAT_2(ihandle, _var)) = { \
        .name       = #ihandle,         \
        .handle     = ihandle,          \
        .cmd        = icmd,             \
    }
#else
#define APP_SHELL_REGISTER(icmd, ihandle)       \
    SECTION_ITEM_REGISTER(APP_SHELL_SECTION, static const app_shell_register_t CONCAT_2(ihandle, _var)) = { \
        .handle     = ihandle,          \
        .cmd        = icmd,             \
    }
#endif

/* Exported typedef ----------------------------------------------------------*/
/** @brief Shell命令处理函数类型定义
 *  @param argv 命令参数数组
 *  @param argc 参数个数
 *  @return 处理结果
 */
typedef uint32_t (*shell_exec_handle_t)(char *argv[], uint32_t argc);

/** @brief Shell命令注册结构体 */
typedef struct {
    #if APP_SHELL_DEBUG
    const char* name;                   /*!< 命令名称 */
    #endif
    const shell_exec_handle_t handle;   /*!< 命令处理函数 */
    const char *cmd;                    /*!< 命令描述 */
} app_shell_register_t;

/* Exported functions --------------------------------------------------------*/

uint32_t app_shell_exec(char *p_shell);

/** @brief Shell命令处理函数
 *  @note 该函数需要在主循环中周期性调用
 */
uint32_t app_shell_process(void *parameter);

/* ---------------------------------------------------------------------------*/
#else
#define APP_SHELL_REGISTER(icmd, ihandle)
app_inline uint32_t app_shell_exec(char *p_shell) { return 0; }
app_inline uint32_t app_shell_process(void *parameter) { return 0; }
#endif
/* ---------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* __APP_SHELL_H__ */

/************************ END OF FILE *****************************************/
