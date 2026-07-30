/*******************************************************************************
File name   : SEGGER_RTT_process.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-11-05
Description : 
History     : 
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SEGGER_RTT_PROCESS_H__
#define __SEGGER_RTT_PROCESS_H__

#ifdef __cplusplus
  extern "C" {
#endif

/* ---------------------------------------------------------------------------*/
#include "app_compiler.h"
#include "app_config.h"
#ifndef USE_SEGGER_RTT
#define USE_SEGGER_RTT                  (0)
#endif

/* Includes ------------------------------------------------------------------*/
#if USE_SEGGER_RTT

/* Exported define -----------------------------------------------------------*/
#define RTT_DBG_PORT                    0

/* Exported macro ------------------------------------------------------------*/
#if 0 /* 后续完善下，先在确认是等级控制，还是单个文件使用宏控制 */
#define SEGGER_RTT_DEBUG_LEVEL          (4)
#define SEGGER_RTT_INFO_LEVEL           (3)
#define SEGGER_RTT_WARN_LEVEL           (2)
#define SEGGER_RTT_ERROR_LEVEL          (1)

#define SEGGER_RTT_LEVEL                (4)
#endif

#if 0
/* RTT 终端号 */
#define RTT_PRINT(type,color,fmt,...)   \
    SEGGER_RTT_printf(RTT_DBG_PORT,"%s%s[%d] "fmt"\r\n%s", color, type, app_time_ms_get(), ##__VA_ARGS__, RTT_CTRL_RESET)
/* 清屏*/
// #define print_clear()                   SEGGER_RTT_WriteString(RTT_DBG_PORT, "\n"RTT_CTRL_CLEAR)
#define print_debug(fmt,...)            RTT_PRINT("D:","",fmt,##__VA_ARGS__)
#define print_info(fmt,...)             RTT_PRINT("I:", RTT_CTRL_TEXT_BRIGHT_GREEN , fmt, ##__VA_ARGS__)
#define print_warn(fmt,...)             RTT_PRINT("W:", RTT_CTRL_TEXT_BRIGHT_YELLOW, fmt, ##__VA_ARGS__)
#define print_error(fmt,...)            RTT_PRINT("E:", RTT_CTRL_TEXT_BRIGHT_RED   , fmt, ##__VA_ARGS__)
#define print_hex(name, bug, size)      RTT_HexPrintf(name, bug, size)
#endif
/* Exported functions --------------------------------------------------------*/
void RTT_Init(void);
void RTT_HexPrintf(uint8_t *name, uint8_t *buf, uint32_t size);
uint32_t RTT_ReadData(char *data, uint32_t *len);
/* ---------------------------------------------------------------------------*/
#else /* USE_SEGGER_RTT */
app_inline void RTT_Init(void) {}
app_inline uint32_t RTT_ReadData(char *data, uint32_t *len) { \
    if (len) *len = 0;              \
    return APP_EOK;                 \
}
#endif /* USE_SEGGER_RTT */
/* ---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __SEGGER_RTT_PROCESS_H__ */

/************************ END OF FILE *****************************************/
