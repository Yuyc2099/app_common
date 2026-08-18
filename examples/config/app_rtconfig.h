/*******************************************************************************
File name   : app_rtconfig.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-12-16
Description : 请详细说明此程序文件完成的主要功能
History     : 修改记录，每条记录应包括修改日期、修改者及修改内容简述
              1. Date:
                  Author:
                  Modification:

*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_RTCONFIG_H__
#define __APP_RTCONFIG_H__

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported define -----------------------------------------------------------*/
#define RT_DEBUG
// #define RT_DEBUG_COLOR

/* Kernel Device Object */
// #define RT_USING_DEVICE
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 256
// #define RT_CONSOLE_DEVICE_NAME "uart1"
// #define ARCH_ARM
// #define ARCH_ARM_CORTEX_M
// #define ARCH_ARM_CORTEX_M0


#define RT_USING_RINGBUFFER
// #define RT_USING_RINGBLK_BUF

/* Exported macro ------------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Private moudle variables --------------------------------------------------*/

/* Private moudle functions --------------------------------------------------*/

/* Private moudle end_flag ---------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __APP_RTCONFIG_H__ */

/************************ END OF FILE *****************************************/
