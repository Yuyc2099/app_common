/*******************************************************************************
File name   : app_stack_wm.c
Author      : Yu You Cheng
Version     : V1.0
Date        : 2026-07-01
Description : MSP 栈水位监测组件
History     : 修改记录，每条记录应包括修改日期、修改者及修改内容简述
              1. Date:
                  Author:
                  Modification:

*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#define DEBUG_LEVEL DBG_LV_INFO
#define DEBUG_TAG   "stack_wm"
#include "app_config.h"

#if USE_APP_STACK_WM
#include "app_stack_wm.h"

/* Private define ------------------------------------------------------------*/
#define STACK_PATTERN   0xA5A5A5A5U

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private moudle variables --------------------------------------------------*/

/* Function declaration ------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

#if defined(__ARMCC_VERSION)
    /* ARM Compiler (Keil MDK)：linker 自动为 AREA 生成 Base/Limit 符号 */
    extern const uint32_t   STACK$$Base;
    extern const uint32_t   STACK$$Limit;
    #define STACK_BOTTOM()  ((uint32_t *)&STACK$$Base)
    #define STACK_TOP()     ((uint32_t *)&STACK$$Limit)
#elif defined(__ICCARM__)
    /* IAR：通过 __section_begin/__section_end 取 CSTACK 段边界 */
    #pragma section="CSTACK"
    #define STACK_BOTTOM()  ((uint32_t *)__section_begin("CSTACK"))
    #define STACK_TOP()     ((uint32_t *)__section_end("CSTACK"))
#elif defined(__GNUC__)
    /* GCC (GNU ld)：链接脚本需导出 _sstack / _estack */
    extern const uint32_t _sstack;
    extern const uint32_t _estack;
    #define STACK_BOTTOM()  ((uint32_t *)&_sstack)
    #define STACK_TOP()     ((uint32_t *)&_estack)
#else
    #error "app_stack_wm: unsupported compiler"
#endif

/* Private moudle functions --------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

void app_stack_wm_fill(void)
{
    uint32_t *bottom = STACK_BOTTOM();
    uint32_t *top    = STACK_TOP();
    uint32_t  total  = (uint32_t)(top - bottom);

    /* 只填充低地址的 NUMER/DENOM，保留上半段给当前调用栈 */
    uint32_t  fill_words = total * APP_STACK_WM_FILL_NUMER / APP_STACK_WM_FILL_DENOM;
    uint32_t *p   = bottom;
    uint32_t *end = bottom + fill_words;

    while (p < end) {
        *p++ = STACK_PATTERN;
    }
}

uint32_t app_stack_wm_get(void)
{
    uint32_t *bottom = STACK_BOTTOM();
    uint32_t *top    = STACK_TOP();
    uint32_t *p      = bottom;

    while (p < top && *p == STACK_PATTERN) {
        p++;
    }

    return (uint32_t)((uint8_t *)top - (uint8_t *)p);
}

void app_stack_wm_show(void)
{
    uint32_t *bottom = STACK_BOTTOM();
    uint32_t *top    = STACK_TOP();
    uint32_t  total  = (uint32_t)((uint8_t *)top - (uint8_t *)bottom);
    uint32_t  used   = app_stack_wm_get();

    print_info("MSP stack: used=%u/%u (%u%%)", used, total, used * 100 / total);
}

#endif /* USE_APP_STACK_WM */

/************************ END OF FILE *****************************************/
