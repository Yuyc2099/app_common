/*******************************************************************************
File name   : vofa.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2026-01-04
Description : 请详细说明此程序文件完成的主要功能 
History     : 修改记录，每条记录应包括修改日期、修改者及修改内容简述
              1. Date: 
                  Author: 
                  Modification: 

*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VOFA_H__
#define __VOFA_H__

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_config.h"
#include "app_compiler.h"

/** @brief Shell功能使能开关 */
#ifndef USE_APP_VOFA
#define USE_APP_VOFA                    0
#endif

#if USE_APP_VOFA
/* Exported define -----------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Private moudle variables --------------------------------------------------*/

/* Private moudle functions --------------------------------------------------*/

/* Private moudle end_flag ---------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  打包 N 个 float + 帧尾后一次性发送
 *
 * @param  data   连续 float 数组首地址
 * @param  n      浮点个数（> VOFA_TX_MAX_FLOATS 将被截断）
 */
void vofa_send_floats(const float *data, uint16_t n);
#else
app_inline void vofa_send_floats(const float *data, uint16_t n) {}
#endif /* USE_APP_VOFA */

#ifdef __cplusplus
}
#endif

#endif /* __VOFA_H__ */

/************************ END OF FILE *****************************************/
