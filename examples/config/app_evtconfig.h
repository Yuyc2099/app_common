/*******************************************************************************
File name   : app_evtconfig.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2026-03-05
Description : 请详细说明此程序文件完成的主要功能
History     : 修改记录，每条记录应包括修改日期、修改者及修改内容简述
              1. Date:
                  Author:
                  Modification:

*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_EVTCONFIG_H__
#define __APP_EVTCONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Exported define -----------------------------------------------------------*/

#define APP_EVTMOD_EXAMPLE              (0x01U << APP_EVTMOD_POS_EXAMPLE)

/* Exported typedef ----------------------------------------------------------*/

typedef enum
{
    APP_EVTMOD_POS_EXAMPLE = 0,
    APP_EVTMOD_POS_MAX = 31,
} app_evtmod_pos_e;

typedef enum
{
    APP_EVTID_EXAMPLE_START = 0,
    APP_EVTID_EXAMPLE_STOP,
} app_evt_example_e;

#ifdef __cplusplus
}
#endif

#endif /* __APP_EVTCONFIG_H__ */

/************************ END OF FILE *****************************************/
