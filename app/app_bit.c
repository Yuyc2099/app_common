/*******************************************************************************
File name   : app_bit.c
Author      : Yu You Cheng
Version     : V1.0
Date        : 2026-05-22
Description : 请详细说明此程序文件完成的主要功能 
History     : 修改记录，每条记录应包括修改日期、修改者及修改内容简述
              1. Date: 
                  Author: 
                  Modification: 

*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "app_bit.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private moudle variables --------------------------------------------------*/

/* Function declaration ------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Private moudle functions --------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  取最高置位 bit（Most Significant Set Bit）
 * @note   通过位传播将最高置位 bit 以下全部填 1，再减去右移 1 位的值得到最高 bit。
 *         例：0x06 (0b0110) → 0x04 (0b0100)
 * @param  x  输入值（0 时返回 0）
 * @retval 仅最高置位 bit 为 1 的 mask
 */
uint32_t BIT_MSB_GET(uint32_t x)
{
    if (x == 0) return 0;

    x |= (x >> 1U);
    x |= (x >> 2U);
    x |= (x >> 4U);
    x |= (x >> 8U);
    x |= (x >> 16U);
    return x - (x >> 1U);
}

/**
 * @brief  清除最高置位 bit
 *         例：0x06 (0b0110) → 0x02 (0b0010)
 * @param  x  输入值
 * @retval 清除最高置位 bit 后的值
 */
uint32_t BIT_MSB_CLR(uint32_t x)
{
    return x & ~BIT_MSB_GET(x);
}

/**
 * @brief  将最高零位置 1（即当前最高置位 bit 的上一位）
 *         例：0x05 (0b0101) → 0x0D (0b1101)
 * @param  x  输入值
 * @retval 置位最高零位后的值
 */
uint32_t BIT_MSB_SET(uint32_t x)
{
    return x | (BIT_MSB_GET(x) << 1U);
}

/**
 * @brief  取最高零位的 mask
 *         例：0x06 (0b0110) → 0x08 (0b1000)
 * @param  x  输入值
 * @retval 仅最高零位为 1 的 mask
 */
uint32_t BIT_MSB_ZERO(uint32_t x)
{
    return BIT_MSB_GET((uint32_t)~x);
}

/**
 * @brief  统计置位 bit 数（popcount / Hamming weight）
 *         例：0x07 (0b0111) → 3
 * @note   使用 SWAR 算法，无分支，固定 4 步完成 32 位计数。
 * @param  x  输入值
 * @retval 置位 bit 的数量（0~32）
 */
uint32_t BIT_COUNT(uint32_t x)
{
    x = x - ((x >> 1U) & 0x55555555u);
    x = (x & 0x33333333u) + ((x >> 2U) & 0x33333333u);
    x = (x + (x >> 4U)) & 0x0F0F0F0Fu;
    return (x * 0x01010101u) >> 24U;
}

/************************ END OF FILE *****************************************/
