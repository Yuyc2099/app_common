/*******************************************************************************
File name   : vofa.c
Author      : Yu You Cheng
Version     : V1.0
Date        : 2026-01-04
Description : 发送格式固定为： [N 个 32 位 float（小端）] + [尾标记 +Inf：0x7F800000]
History     : 修改记录，每条记录应包括修改日期、修改者及修改内容简述
              1. 

*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#define DEBUG_LEVEL DBG_LV_ERROR
#define DEBUG_TAG   "vofa"

#include "vofa.h"
#if USE_APP_VOFA

/* Private define ------------------------------------------------------------*/
/**
 * @brief  单帧允许发送的最大 float 个数
 */
#ifndef VOFA_TX_MAX_FLOATS
#define VOFA_TX_MAX_FLOATS              (16u)
#endif

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/**
 * @brief VOFA+ 帧尾标记（IEEE754 单精度 +Inf）
 *
 * 小端发送字节序：00 00 80 7F
 * VOFA+ 通过该特殊值识别一帧数据的结束位置。
 */
static const uint32_t s_vofa_tail = 0x7F800000;
 
/**
 * @brief 发送打包缓冲区
 *
 * 用于存放：N 个 float 数据 + 4 字节帧尾。
 */
static uint8_t s_vofa_tx_buf[VOFA_TX_MAX_FLOATS * sizeof(float) + sizeof(s_vofa_tail)];

/* Private moudle variables --------------------------------------------------*/

/* Function declaration ------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static uint32_t vofa_init(void)
{
    app_print_enable(false);
    return 0;
}
APP_APPLICATION_INIT(vofa_init);

/* Private moudle functions --------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
app_weak void vofa_send(const char *p_dat, uint16_t len)
{
    print_error("weak function: vofa_send!!!");
}


/**
 * @brief  打包 N 个 float + 帧尾后一次性发送
 *
 * @param  data   连续 float 数组首地址
 * @param  n      浮点个数（> VOFA_TX_MAX_FLOATS 将被截断）
 */
void vofa_send_floats(const float *data, uint16_t n)
{
    if (n > VOFA_TX_MAX_FLOATS) {
        n = VOFA_TX_MAX_FLOATS;
    }
 
    uint16_t bytes = n * sizeof(float);
    memcpy(s_vofa_tx_buf, data, bytes);
    for (uint8_t i = 0; i < sizeof(s_vofa_tail); i++) {
        s_vofa_tx_buf[bytes + i] = (uint8_t)(s_vofa_tail >> (i * 8));
    }
    bytes += sizeof(s_vofa_tail);

    vofa_send((char *)s_vofa_tx_buf, bytes);
}
#endif /* USE_APP_VOFA */

/************************ END OF FILE *****************************************/
