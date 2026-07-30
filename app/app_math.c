/*******************************************************************************
File name   : app_math.c
Author      : Yu You Cheng
Version     : V1.0
Date        : 2026-02-05
Description : 请详细说明此程序文件完成的主要功能 
History     : 修改记录，每条记录应包括修改日期、修改者及修改内容简述
              1. Date: 
                  Author: 
                  Modification: 

*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "app_config.h"
#if USE_APP_MATH
#include "app_math.h"
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private moudle variables --------------------------------------------------*/

/* Function declaration ------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  快速倒数平方根核心函数 (计算 1.0 / sqrt(x))
 * 
 * @param x 
 * @return float 
 */
static float fast_inverse_sqrtf(float x)
{
    union { float f; int i; } u;
    u.f = x;
    u.i = 0x5f3759df - (u.i >> 1);
    // 二次次牛顿迭代（测试100K，误差+-0.0012）
    u.f = u.f * (1.5f - 0.5f * x * u.f * u.f);
    u.f = u.f * (1.5f - 0.5f * x * u.f * u.f);
    return u.f;
}

/* Private moudle functions --------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/**
 * @brief 基于快速倒数平方根实现的快速平方根（替代sqrtf）
 * 
 * @param x 
 * @return float 
 */
float app_fast_sqrtf(float x) {
    if (x <= 0.0f) {
        return 0.0f;
    }
    return x * fast_inverse_sqrtf(x);
}

#endif /* USE_APP_MATH */
/************************ END OF FILE *****************************************/
