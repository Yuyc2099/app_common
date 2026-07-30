/*******************************************************************************
File name   : app_common.c
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-10-21
Description : 通用功能模块实现文件
History     : 

*******************************************************************************/

/** @file app_common.c
 *  @brief 通用功能模块实现
 *  @details 实现系统通用的工具函数，包括断言处理、延时函数、字符串处理等
 */

/* Includes ------------------------------------------------------------------*/
#define DEBUG_LEVEL DBG_LV_DEBUG
#define DEBUG_TAG   "common"
#include "app_config.h"

/* ---------------------------------------------------------------------------*/
/**
 * @brief 断言失败处理函数
 * @param file 文件名
 * @param line 行号
 * @note 此函数会打印错误信息并触发死循环，在支持的情况下会打印调用栈
 */
void app_assert_failed(uint8_t* file, uint32_t line)
{
    print_error("%s %d", file, line);
    #if USE_CM_BACKTRACE
    cm_backtrace_assert(cmb_get_sp());
    #endif
    while(1);
}

/**
 * @brief 重启应用程序
 * @note   调用此函数将导致系统中断，所有未保存的数据可能会丢失。
 *         在调用前应确保所有关键操作已完成。
 */
void app_reboot(void)
{
    print_error("%s %d", __func__, __LINE__);
    __disable_irq();
    NVIC_SystemReset(); 
    while(1);
}

/* ---------------------------------------------------------------------------*/
#ifndef APP_IRQ_PERF_DEBUG_THRESHOLD
#define APP_IRQ_PERF_DEBUG_THRESHOLD    (10)
#endif

static volatile uint32_t s_irq_disable_time = 0;

/**
 * @brief 禁用中断并记录状态
 * @return uint32_t 之前的中断状态(PRIMASK寄存器值)
 * @note 返回值必须传递给app_interrupt_enable()以恢复中断状态
 */
uint32_t app_interrupt_disable(void)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    if (primask == 0) {
        s_irq_disable_time = app_time_ms_get();
    }
    return primask;
}

/**
 * @brief 恢复中断状态并检查性能
 * @param primask 之前保存的中断状态
 * @note 参数必须是app_interrupt_disable()的返回值
 */
void app_interrupt_enable(uint32_t primask)
{
    __set_PRIMASK(primask);
    if (primask == 0) {
        uint32_t disable_duration = app_time_ms_diff(app_time_ms_get(), s_irq_disable_time);
        if (disable_duration > APP_IRQ_PERF_DEBUG_THRESHOLD) {
            print_error("Interrupt disabled for %ums, return address: %p", disable_duration, app_return_address(0));
        }
    }
}

/* ---------------------------------------------------------------------------*/
#ifndef APP_TICK_PER_SECOND
#define APP_TICK_PER_SECOND             (1000)
#endif

#if (1000 % APP_TICK_PER_SECOND != 0u)
#warning "app common 1000 % APP_TICK_PER_SECOND != 0u."
#endif

static volatile uint32_t s_app_tick = 0;

/**
 * @brief 增加系统毫秒计数器
 * @note 此函数是中断安全的，会临时禁用中断
 */
void app_tick_increase(void)
{
    // uint32_t level = app_interrupt_disable();
    s_app_tick++;
    // app_interrupt_enable(level);
}

/**
 * @brief 获取当前系统滴答值
 * @return uint32_t 当前滴答计数
 * @note 返回值是单调递增的，溢出后会从0重新开始
 */
uint32_t app_tick_get(void)
{
    return s_app_tick;
}

/**
 * @brief 获取系统时间戳(毫秒)
 * @return uint32_t 当前时间戳(毫秒)
 * @note 时间戳基于系统滴答计数，精度由APP_TICK_PER_SECOND决定
 */
app_weak uint32_t app_time_ms_get(void)
{
    return app_tick_get() * (1000u / APP_TICK_PER_SECOND);
}

/**
 * @brief 将毫秒转换为滴答数
 * @param ms 毫秒时间
 * @return uint32_t 对应的滴答数
 * @note 如果输入为负数，返回0xffffffff
 */
uint32_t app_tick_get_from_ms(int32_t ms)
{
    if (ms < 0) {
        return 0xffffffff;
    }
#if (APP_TICK_PER_SECOND == 1000u)
    return ms;
#else
    uint32_t tick = APP_TICK_PER_SECOND * (ms / 1000);
    tick += (APP_TICK_PER_SECOND * (ms % 1000) + 999) / 1000;
    return tick;
#endif
}

/**
 * @brief 计算两个时间戳之间的差值
 * @param now_ms 当前时间戳(ms)
 * @param prev_ms 之前的时间戳(ms)
 * @return 时间差(ms)
 * @note 支持时间戳溢出的情况
 */
uint32_t app_time_ms_diff(uint32_t now_ms, uint32_t prev_ms)
{
    return ((now_ms - prev_ms) & 0xffffffff);
}

/**
 * @brief uint32_t绝对值差值计算
 * @return a和b的差值绝对值
 */
uint32_t app_compare_uint32_diff(int32_t a, int32_t b)
{
    return ((a - b) & 0xffffffff);
}

/**
 * @brief int32_t绝对值差值计算
 * @return a和b的差值绝对值
 */
uint32_t app_compare_int32_diff(int32_t a, int32_t b)
{
    uint32_t ua = a, ub = b;
    return (a >= b) ? (ua - ub) : (ub - ua);
}

/* ---------------------------------------------------------------------------*/

/**
 * @brief 微秒级延时函数
 * @param us 延时时间(us)
 * @note 使用SysTick实现精确延时，延时精度依赖于系统时钟配置
 */
void app_delay_us(uint32_t us)
{ 
    /* us延时异常时核对HSI_VALUE */
    uint8_t fac_us = APP_HSI_VALUE / 1000000;
    uint32_t ticks = us*fac_us;         //需要的节拍数
    uint32_t reload = APP_SYSTICK_LOAD; //LOAD的值
    uint32_t told = APP_SYSTICK_VAL;    //刚进入时的计数器值
    uint32_t tnow = 0, tcnt = 0;

    while(1) {
        tnow = APP_SYSTICK_VAL;
        if (tnow != told) {
            // SYSTICK是一个递减的计数器
            if (tnow < told) {
                tcnt += told - tnow;
            } else { 
                tcnt += reload - tnow + told;
            }
            told = tnow;
            // 时间超过/等于要延迟的时间,则退出.
            if (tcnt >= ticks) {
                break;
            }
        }
    }
}

/**
 * @brief 毫秒级延时函数
 * @param ms 延时时间(ms)
 * @note 实际延时精度依赖于app_delay_us()的实现
 */
void app_delay_ms(uint32_t ms)
{ 
    app_delay_us(ms*1000);
}

/* ---------------------------------------------------------------------------*/
/**
 * @brief 分割字符串为多个子串
 * @param input 输入字符串（将被修改）
 * @param tokens 存储分割结果的指针数组
 * @param max_tokens 最大分割数量
 * @return 实际分割出的子串数量
 * @details
 *  - 以空格为分隔符
 *  - 连续空格会被当作一个分隔符
 *  - 输入字符串会被修改（空格被替换为'\0'）
 *  - tokens数组中存储的是指向原字符串中各个子串的指针
 */
uint32_t app_string_split(char *input, char **tokens, uint32_t max_tokens)
{
    uint32_t count = 0;                 // 当前分割出的子串数量
    char *start = input;                // 当前子串起始位置
    uint32_t in_token = 0;              // 是否在子串中
    
    // 遍历字符串的每个字符
    for (char *p = input; *p != '\0' && count < max_tokens; p++) {
        if (isspace((unsigned char)*p)) {
            // 遇到空格，结束当前子串
            if (in_token) {
            *p = '\0';                  // 将空格替换为字符串结束符
                tokens[count++] = start;// 保存当前子串
                in_token = 0;
            }
        } else {
            // 遇到非空格字符
            if (!in_token) {
                start = p;  // 新子串开始
                in_token = 1;
            }
        }
    }
    
    // 处理最后一个子串
    if (in_token && count < max_tokens) {
        tokens[count++] = start;
    }
    
    return count;
}


const char* file_name_get(const char* p_fpath)
{
    const char* ptr = p_fpath + strlen((const char*)p_fpath);
    while (ptr >= p_fpath) {
        if (*ptr == '/' || *ptr == '\\') {
            ptr++;
            break;
        }
        ptr--;
    }
    return ptr;
}


/**
 * @brief CRC-32 扩展计算（支持分段计算）
 * @param p_data   输入数据指针
 * @param data_len 数据长度（字节）
 * @param crc      CRC 初始值
 * @return         本次数据的 CRC-32 校验值
 * @details
 *  - 多项式：X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X+1
 *  - 多项式系数 0xEDB88320, 结果异或值 0xFFFFFFFF
 */
uint32_t app_crc32_ext(uint8_t const * p_data, uint32_t data_len, uint32_t crc)
{
    while (data_len--) {
        crc = crc ^ p_data[0];
        p_data++;
        for (uint32_t j=8; j>0; j--) {
            crc = (crc >> 1) ^ (0xEDB88320U & ((crc & 1) ? 0xFFFFFFFF : 0));
        }
    }
    return ~crc;
}

/**
 * @brief CRC-32 计算（初始值固定为 0xFFFFFFFF）
 * @param p_data   输入数据指针
 * @param data_len 数据长度（字节）
 * @return         CRC-32 校验值
 */
uint32_t app_crc32(uint8_t const * p_data, uint32_t data_len)
{
    return app_crc32_ext(p_data, data_len, 0xFFFFFFFFU);
}


/************************ END OF FILE *****************************************/
