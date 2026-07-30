/*******************************************************************************
File name   : app_stack_wm.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2026-07-01
Description : MSP 栈水位监测组件
History     : 修改记录，每条记录应包括修改日期、修改者及修改内容简述
              1. Date:
                  Author:
                  Modification:

*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_STACK_WM_H__
#define __APP_STACK_WM_H__

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_compiler.h"
#include "app_config.h"

/* Exported define -----------------------------------------------------------*/

/** @brief 栈水位监测使能，0 = 禁用 */
#ifndef USE_APP_STACK_WM
#define USE_APP_STACK_WM                0
#endif

/**
 * @brief 填充比例（分子/分母），默认填充低地址的 1/2。
 *        仅填充下半段，保留上半段给当前调用栈，避免覆盖自身帧。
 *        范围：1/4 ~ 3/4，不建议超过 3/4。
 */
#ifndef APP_STACK_WM_FILL_NUMER
#define APP_STACK_WM_FILL_NUMER         1
#endif
#ifndef APP_STACK_WM_FILL_DENOM
#define APP_STACK_WM_FILL_DENOM         2
#endif

/* Exported functions --------------------------------------------------------*/
#if USE_APP_STACK_WM

/**
 * @brief  用魔数填充 MSP 栈的低地址部分（比例由 FILL_NUMER/DENOM 决定）。
 * @note   需在系统启动早期、栈使用量最少时调用（如 main 入口处）。
 */
void app_stack_wm_fill(void);

/**
 * @brief  获取 MSP 栈历史最大使用字节数（高水位）。
 * @retval 已使用的字节数；若未调用 app_stack_wm_fill 则结果无效。
 */
uint32_t app_stack_wm_get(void);

/**
 * @brief  打印栈水位信息（使用 print_info）。
 */
void app_stack_wm_show(void);

#else  /* USE_APP_STACK_WM == 0 */

#define app_stack_wm_fill()     ((void)0)
#define app_stack_wm_get()      (0U)
#define app_stack_wm_show()     ((void)0)

#endif /* USE_APP_STACK_WM */

#ifdef __cplusplus
}
#endif

#endif /* __APP_STACK_WM_H__ */

/************************ END OF FILE *****************************************/
