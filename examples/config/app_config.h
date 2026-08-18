/*******************************************************************************
File name   : app_config.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-10-28
Description :
History     :
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#ifdef __cplusplus
  extern "C" {
#endif

/* RT_THREAD LIBRARY ---------------------------------------------------------*/
#include "rtdevice.h"
#include "rtthread.h"

/* ---------------------------------------------------------------------------*/
/* APP_COMMON ----------------------------------------------------------------*/
#define USE_APP_COMMON                  1
/* ---------------------------------------------------------------------------*/
#if	    USE_APP_COMMON
    #include "py32f0xx.h"
    #define  APP_HSI_VALUE                      (48000000U)  // HSI Value in Hz (used as delay function source)
    #define  APP_SYSTICK_LOAD                   (SysTick->LOAD)
    #define  APP_SYSTICK_VAL                    (SysTick->VAL)

    #include "app_compiler.h"
    #include "app_common.h"
    #include "app_error.h"
    #include "app_section.h"
    #include "app_bit.h"
#endif

/* SEGGER_RTT ----------------------------------------------------------------*/
#define USE_SEGGER_RTT                  0
/* ---------------------------------------------------------------------------*/
#if     USE_SEGGER_RTT
    #define SEGGER_RTT_MAX_NUM_UP_BUFFERS       (1)     // Max. number of up-buffers (T->H) available on this target    (Default: 3)
    #define SEGGER_RTT_MAX_NUM_DOWN_BUFFERS     (1)     // Max. number of down-buffers (H->T) available on this target  (Default: 3)
    #define BUFFER_SIZE_UP                      (2048)  // Size of the buffer for terminal output of target, up to host (Default: 1k)
    #define BUFFER_SIZE_DOWN                    (128)   // Size of the buffer for terminal input to target from host (Usually keyboard input) (Default: 16)
    #define SEGGER_RTT_PRINTF_BUFFER_SIZE       (64u)   // Size of buffer for RTT printf to bulk-send chars via RTT     (Default: 64)

    #include "SEGGER_RTT.h"
#endif
#include "SEGGER_RTT_process.h"

/* APP_INIT -----------------------------------------------------------------*/
#define USE_APP_INIT                    1
/* ---------------------------------------------------------------------------*/
#include "app_init.h"

/* APP_SHELL -----------------------------------------------------------------*/
#define USE_APP_SHELL                   1
/* ---------------------------------------------------------------------------*/
#if     USE_APP_SHELL
    #define APP_SHELL_MAX_BUFF_SIZE             (128)
    #define APP_SHELL_MAX_TOKENS                (8)
#endif
#include "app_shell.h"

/* APP_TIMER -----------------------------------------------------------------*/
#define USE_APP_TIMER                   1
/* ---------------------------------------------------------------------------*/
#if     USE_APP_TIMER
    #define APP_HARD_TIMER_ENABLE               1
    #define APP_TIMER_DELAY_EXTEND              1
#endif

#include "app_timer.h"

/* APP_THREAD ----------------------------------------------------------------*/
#define USE_APP_THREAD                  1
/* ---------------------------------------------------------------------------*/
#include "app_thread.h"

/* cm_backtrace --------------------------------------------------------------*/
#define USE_CM_BACKTRACE                0      // 不使用时cmb_fault.S不能参与编译
/* ---------------------------------------------------------------------------*/
#if     USE_CM_BACKTRACE
    #define ADDR2LINE_PATH                      "D:\\Programs\\mingw_w64_gcc_14_3\\bin\\addr2line.exe"
    /* print line, must config by user */
    #define cmb_println(fmt, ...)               do { \
        kprintf("\033[1;31m"); kprintf(fmt, ##__VA_ARGS__); kprintf("\r\n\033[0m"); \
    } while(0)

    /* enable bare metal(no OS) platform */
    #define CMB_USING_BARE_METAL_PLATFORM
    /* cpu platform type, must config by user */
    #define CMB_CPU_PLATFORM_TYPE       CMB_CPU_ARM_CORTEX_M0
    /* enable dump stack information */
    #define CMB_USING_DUMP_STACK_INFO
#endif
#include "cm_backtrace.h"

/* APP_PRINT -----------------------------------------------------------------*/
#if     (!USE_SEGGER_RTT)
    #define USE_UART_PRINTF             1
    #if     USE_UART_PRINTF
    #define USE_RT_THREAD_PRINTF        1
    #endif
#endif
#include "app_print.h"
/* ---------------------------------------------------------------------------*/

/* APP_VOFA -----------------------------------------------------------------*/
#define USE_APP_VOFA                    0
#include "vofa.h"
/* ---------------------------------------------------------------------------*/

/* APP_MATH -----------------------------------------------------------------*/
#define USE_APP_MATH                    0
#include "app_math.h"
/* ---------------------------------------------------------------------------*/

/* APP_EVENT -----------------------------------------------------------------*/
#define USE_APP_EVENT                   1
#define APP_EVENT_DEBUG                 1
#define APP_MAX_EVENT_SUBSCRIBERS       (16)
#include "app_event.h"
#include "app_evtconfig.h"
/* ---------------------------------------------------------------------------*/

/* APP_SAVR_DATA -------------------------------------------------------------*/
#define USE_APP_SAVE_DATA               0
#include "app_save_data.h"
/* ---------------------------------------------------------------------------*/

/* APP_STACK -----------------------------------------------------------------*/
#define USE_APP_STACK_WM                1   // 1: 使能 MSP 栈水位监测
/* APP_STACK_WM_FILL_NUMER/DENOM 控制填充比例 */
#define APP_STACK_WM_FILL_NUMER         1
#define APP_STACK_WM_FILL_DENOM         2
#include "app_stack_wm.h"
/* ---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __APP_CONFIG_H__ */

/************************ END OF FILE *****************************************/
