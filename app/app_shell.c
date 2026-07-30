/*******************************************************************************
File name   : app_shell.c
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-11-05
Description : Shell命令处理模块实现文件
History     : 

*******************************************************************************/

/** @file app_shell.c
 *  @brief Shell命令处理模块实现
 *  @details 实现Shell命令的注册和处理功能，支持命令解析和执行
 */
/* Includes ------------------------------------------------------------------*/
#define DEBUG_LEVEL DBG_LV_DEBUG
#define DEBUG_TAG   "shell"

#include "app_config.h"
#if USE_APP_SHELL
#include "app_shell.h"

/* Private variables ---------------------------------------------------------*/
/** @brief Shell命令注册段定义 */
SECTION_DEF(APP_SHELL_SECTION, app_shell_register_t);

/** @brief Shell命令缓冲区 */
static char s_shell_cmd_buff[APP_SHELL_MAX_BUFF_SIZE] = {0};

/* Exported functions --------------------------------------------------------*/
app_weak uint32_t app_shell_read_data(char *data, uint32_t *len)
{
    print_error("weak function: app_shell_read_data!!!");
    return APP_EEMPTY;
}

uint32_t app_shell_exec(char *p_shell)
{
    if (p_shell == NULL) {
        return APP_ERROR;
    }

    char  *tokens[APP_SHELL_MAX_TOKENS] = {0};
    uint32_t tokens_num = app_string_split(s_shell_cmd_buff, tokens, APP_SHELL_MAX_TOKENS);
    if (tokens_num == 0) {
        return APP_ERROR;
    }

    #if APP_SHELL_DEBUG
    print_debug("shell tokens num:%d", tokens_num);
    for (uint8_t i=0; i<tokens_num; i++) {
        print_debug("[%02d] cmd:%s", i, tokens[i]);
    }
    #endif

    const uint32_t *p_start = (const uint32_t*)SECTION_START_ADDR(APP_SHELL_SECTION);
    const uint32_t *p_end = (const uint32_t*)SECTION_END_ADDR(APP_SHELL_SECTION);
    app_shell_register_t *p_handle;
    while (p_start < p_end) {
        p_handle = (app_shell_register_t*)p_start;
        if (p_handle && p_handle->handle && p_handle->cmd && strcmp(p_handle->cmd, tokens[0]) == 0) {
            #if APP_SHELL_DEBUG
            print_debug("execution: %s, cmd:%s", p_handle->name, tokens[0]);
            #endif
            /* 移除 p_handle->cmd */
            p_handle->handle(&tokens[1], tokens_num - 1);
        }
        p_start = section_next_item(p_start, p_end, sizeof(app_shell_register_t));
    }
    return APP_EOK;
}

/** @brief Shell命令处理函数
 *  @details 处理接收到的Shell命令，包括：
 *           1. 从输入设备读取命令
 *           2. 解析命令参数
 *           3. 查找并执行对应的处理函数
 *  @note 该函数需要在主循环中周期性调用
 */
uint32_t app_shell_process(void *parameter)
{
    uint32_t recv_len = sizeof(s_shell_cmd_buff) - 1;
    if (app_shell_read_data(s_shell_cmd_buff, &recv_len) != APP_EOK) {
        return APP_ERROR;
    }
    return app_shell_exec(s_shell_cmd_buff);
}
APP_THREAD_REGISTER(app_shell_process, NULL, APP_THREAD_LOW_LEVEL);

#endif /* USE_APP_SHELL */
/************************ END OF FILE *****************************************/
