/*******************************************************************************
Copyright   : 深圳市爱都科技有限公司
File name   : app_save_data.h
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
#ifndef __APP_SAVE_DATA_H__
#define __APP_SAVE_DATA_H__

#ifdef __cplusplus
  extern "C" {
#endif

/* ---------------------------------------------------------------------------*/
#include "app_compiler.h"
#include "app_config.h"
#ifndef USE_APP_SAVE_DATA
#define USE_APP_SAVE_DATA               0
#endif
/* Includes --------------------------------------------------------------------*/
#if USE_APP_EVENT
#include "app_common.h"
#endif

/* Exported typedef ----------------------------------------------------------*/
typedef struct
{
    uint16_t key;
    uint16_t data_size;
    uint32_t block_offset;
}app_data_size_map_t;

/* Exported functions ---------------------------------------------------------*/
#if USE_APP_EVENT
/**
 * @brief   微信支付保存数据初始化
 * @retval  APP_EXXX
*/
uint32_t app_save_data_init(void);

/**
 * @brief   微信支付保存数据读取
 * @note    读取所有的数据，返回字节数为实际读取的长度
 * @param[int-out] *read_size 传入data的可使用字节数，返回为实际读取到的数据大小
 * @retval APP_EXXX
*/
uint32_t app_save_data_read(uint16_t index, void *data, uint16_t *data_size);

/**
 * @brief   微信支付保存数据写入
 * @retval  APP_EXXX
*/
uint32_t app_save_data_write(uint16_t index,void *data,uint16_t data_size);

/**
 * @brief   微信支付保存数据清除
 * @retval  APP_EXXX
 */
uint32_t app_save_data_clear(uint16_t index);
/**
 * @brief 微信支付保存数据是否存在
 * @retval  APP_EXXX
*/
uint32_t app_save_data_exist(uint16_t index);
/* ---------------------------------------------------------------------------*/
#else
app_inline uint32_t app_save_data_init(void) { return APP_EOK;}
app_inline uint32_t app_save_data_read(uint16_t index, void *data, uint16_t *data_size) { return APP_EOK; }
app_inline uint32_t app_save_data_write(uint16_t index,void *data,uint16_t data_size) { return APP_EOK; }
app_inline uint32_t app_save_data_clear(uint16_t index) { return APP_EOK; }
app_inline uint32_t app_save_data_exist(uint16_t index) { return APP_EOK; }
#endif /* __APP_EVENT_H__ */
/* ---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /*__APP_SAVE_DATA_H__*/

/************************ END OF FILE *****************************************/
