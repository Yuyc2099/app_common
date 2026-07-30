/*******************************************************************************
File name   : app_bit.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2026-05-22
Description : 通用位操作工具宏定义，包含MASK生成、LSB/MSB操作、位置位/清除/判断、位计数等功能
History     : 修改记录，每条记录应包括修改日期、修改者及修改内容简述
              1. Date: 
                  Author: 
                  Modification: 

*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_BIT_H__
#define __APP_BIT_H__

#ifdef __cplusplus
  extern "C" {
#endif
/* ---------------------------------------------------------------------------*/
#include "app_compiler.h"

/* Includes ------------------------------------------------------------------*/

/* Exported define -----------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * Mask 操作
 * -------------------------------------------------------------------------- */

/** @brief 生成连续置位 mask，[h:l] 位为 1，例：BIT_MASK_GEN(3,1) = 0x0E */
#define BIT_MASK_GEN(h, l)          (((~0UL) - (1UL << (l)) + 1UL) & (~0UL >> (31U - (h))))

/** @brief 判断 mask 中所有 bit 是否全部置位，例：BIT_MASK_IS_SET(0x07, 0x05) = 1 */
#define BIT_MASK_IS_SET(x, mask)    (((x) & (mask)) == (mask))

/** @brief 置位 mask 中所有 bit，例：BIT_MASK_SET(0x02, 0x05) = 0x07 */
#define BIT_MASK_SET(x, mask)       ((x) | (mask))

/** @brief 清除 mask 中所有 bit，例：BIT_MASK_CLR(0x07, 0x05) = 0x02 */
#define BIT_MASK_CLR(x, mask)       ((x) & ~(mask))

/* --------------------------------------------------------------------------
 * LSB (Least Significant Set Bit) 操作
 * -------------------------------------------------------------------------- */

/** @brief 取最低置位 bit，例：BIT_LSB_GET(0x06) = 0x02 */
#define BIT_LSB_GET(x)          ((x) & -(x))

/** @brief 清除最低置位 bit，例：BIT_LSB_CLR(0x06) = 0x04 */
#define BIT_LSB_CLR(x)          ((x) & ((x) - 1U))

/** @brief 将最低零位置 1，例：BIT_LSB_SET(0x05) = 0x07 */
#define BIT_LSB_SET(x)          ((x) | ((x) + 1U))

/** @brief 取最低零位的 mask，例：BIT_LSB_ZERO(0x06) = 0x01 */
#define BIT_LSB_ZERO(x)         (~(x) & ((x) + 1U))

/* --------------------------------------------------------------------------
 * MSB (Most Significant Set Bit) 操作
 * -------------------------------------------------------------------------- */

/** @brief 取最高置位 bit，例：BIT_MSB_GET(0x06) = 0x04 */
uint32_t BIT_MSB_GET(uint32_t x);

/** @brief 清除最高置位 bit，例：BIT_MSB_CLR(0x06) = 0x02 */
uint32_t BIT_MSB_CLR(uint32_t x);

/** @brief 将最高零位置 1，例：BIT_MSB_SET(0x05) = 0x0D */
uint32_t BIT_MSB_SET(uint32_t x);

/** @brief 取最高零位的 mask，例：BIT_MSB_ZERO(0x06) = 0x08 */
uint32_t BIT_MSB_ZERO(uint32_t x);

/* --------------------------------------------------------------------------
 * 通用 bit 判断与计数
 * -------------------------------------------------------------------------- */

/** @brief 判断 x 是否为 2 的幂（非零），例：BIT_IS_POW2(0x04) = 1 */
#define BIT_IS_POW2(x)          (((x) != 0U) && (((x) & ((x) - 1U)) == 0U))

/** @brief 判断指定 bit 是否置位，例：BIT_IS_SET(0x06, 1) = 1 */
#define BIT_IS_SET(x, n)        (((x) >> (n)) & 1U)

/** @brief 置位指定 bit */
#define BIT_SET(x, n)           ((x) | (1UL << (n)))

/** @brief 清除指定 bit */
#define BIT_CLR(x, n)           ((x) & ~(1UL << (n)))

/** @brief 翻转指定 bit */
#define BIT_FLIP(x, n)          ((x) ^ (1UL << (n)))

/** @brief 统计置位 bit 数（popcount），例：BIT_COUNT(0x07) = 3 */
uint32_t BIT_COUNT(uint32_t x);

/* Exported typedef ----------------------------------------------------------*/

/* Private moudle variables --------------------------------------------------*/

/* Private moudle functions --------------------------------------------------*/

/* Private moudle end_flag ---------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __APP_BIT_H__ */

/************************ END OF FILE *****************************************/
