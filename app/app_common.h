/*******************************************************************************
File name   : app_common.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-09-25
Description : 通用功能模块头文件
History     : 
*******************************************************************************/

/** @file app_common.h
 *  @brief 通用功能模块
 *  @details 提供系统通用的宏定义、工具函数和断言功能
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_compiler.h"

/* Exported define -----------------------------------------------------------*/
/** @brief 断言功能使能开关 */
#define USE_APP_ASSERT                  1 // 1: enable assert, 0: disable assert

/* Exported macro ------------------------------------------------------------*/


#define __YEAR__    ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 + (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))
#define __MONTH__   ( __DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 1 : 6) \
                    : __DATE__ [2] == 'b' ? 2 \
                    : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4) \
                    : __DATE__ [2] == 'y' ? 5 \
                    : __DATE__ [2] == 'n' ? 6 \
                    : __DATE__ [2] == 'l' ? 7 \
                    : __DATE__ [2] == 'g' ? 8 \
                    : __DATE__ [2] == 'p' ? 9 \
                    : __DATE__ [2] == 't' ? 10 \
                    : __DATE__ [2] == 'v' ? 11 : 12)
#define __DAY__     ((__DATE__ [4] == ' ' ? 0 : ((__DATE__ [4] - '0') * 10 )) + (__DATE__ [5] - '0'))
#define __HOUR__    ((__TIME__ [0] == ' ' ? 0 : ((__TIME__ [0] - '0') * 10 )) + (__TIME__ [1] - '0'))
#define __MINUTE__  ((__TIME__ [3] == ' ' ? 0 : ((__TIME__ [3] - '0') * 10 )) + (__TIME__ [4] - '0'))
#define __SECOND__  ((__TIME__ [6] == ' ' ? 0 : ((__TIME__ [6] - '0') * 10 )) + (__TIME__ [7] - '0'))

/** @brief 连接两个宏参数 */
#ifndef CONCAT_2
#define CONCAT_2(p1, p2)                p1##p2
#endif

/** @brief 将宏参数转换为字符串 */
#ifndef STRINGIFY
#define STRINGIFY(val)                  #val
#endif

/** @brief 4字节对齐 */
#ifndef ALIGN_4_BYTE
#define ALIGN_4_BYTE(n)                 (((n) +  3) & 0xfffffffc)
#endif

/** @brief 8字节对齐 */
#ifndef ALIGN_8_BYTE
#define ALIGN_8_BYTE(n)                 (((n) +  7) & 0xfffffff8)
#endif

/** @brief 16字节对齐 */
#ifndef ALIGN_16_BYTE
#define ALIGN_16_BYTE(n)                (((n) + 15) & 0xfffffff0)
#endif

/** @brief 32字节对齐 */
#ifndef ALIGN_32_BYTE
#define ALIGN_32_BYTE(n)                (((n) + 31) & 0xffffffe0)
#endif

/** @brief 64字节对齐 */
#ifndef ALIGN_64_BYTE
#define ALIGN_64_BYTE(n)                (((n) + 63) & 0xffffffc0)
#endif

/** @brief 计算数组元素个数 */
#ifndef ARRAY_COUNT
#define ARRAY_COUNT(X)                  (sizeof(X) / sizeof(X[0]))
#endif

/** @brief 获取较小值 */
#ifndef MIN
#define MIN(x,y)                        (((x)<(y))?(x):(y))
#endif

/** @brief 获取较大值 */
#ifndef MAX
#define MAX(x,y)                        (((x)>(y))?(x):(y))
#endif

/** @brief 获取绝对值 */
#ifndef ABS
#define ABS(x)                          (((x)>=0)?(x):-(x))
#endif

/** @brief 获取结构体成员偏移 */
#ifndef OFFSETOF
#define OFFSETOF(T, x)                  ((size_t) &((T *)0)->x)
#endif

/** @brief 根据成员指针获取结构体指针 */
#ifndef CONTAINER_OF
#define CONTAINER_OF(ptr, type, member) ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))
#endif

/** @brief 将枚举值转换为字符串 */
#ifndef CASE_TO_NAME
#define CASE_TO_NAME(x)                 case(x): return #x;
#endif

/** @brief 断言宏 */
#if (defined(USE_APP_ASSERT) && USE_APP_ASSERT)
#define APP_ASSERT(expr) ((expr) ? (void)0 : app_assert_failed((uint8_t *)file_name_get(__FILE__), __LINE__))
#else
#define APP_ASSERT(expr) ((void)0)
#endif /* USE_APP_ASSERT */

/** @brief 调试函数信息打印 */
#define debug_func()                    print_debug("%s:%d", __func__, __LINE__)

/* Exported functions --------------------------------------------------------*/
/**
 * @brief 获取段中下一个条目
 * @param p_item 当前条目指针
 * @param p_end 段结束指针
 * @param item_size 条目大小
 * @return 下一个条目指针
 */
app_inline const uint32_t* section_next_item(const uint32_t *p_item, const uint32_t* p_end, uint32_t item_size)
{
    return p_item + (item_size / sizeof(uint32_t*));
}

/**
 * @brief 断言失败处理函数
 * @param file 文件名
 * @param line 行号
 */
void app_assert_failed(uint8_t* file, uint32_t line);

/**
 * @brief 重启应用程序
 * @note   调用此函数将导致系统中断，所有未保存的数据可能会丢失。
 *         在调用前应确保所有关键操作已完成。
 */
void app_reboot(void);

/**
 * @brief 禁用中断并记录状态
 * @return uint32_t 之前的中断状态(PRIMASK寄存器值)
 */
uint32_t app_interrupt_disable(void);

/**
 * @brief 恢复中断状态并检查性能
 * @param primask 之前保存的中断状态
 */
void app_interrupt_enable(uint32_t primask);

/**
 * @brief 增加系统毫秒计数器
 * @note 此函数是中断安全的，会临时禁用中断
 */
void app_tick_increase(void);

/**
 * @brief 获取当前系统滴答值
 * @return uint32_t 当前滴答计数
 */
uint32_t app_tick_get(void);

/**
 * @brief 获取系统时间戳(毫秒)
 * @return uint32_t 当前时间戳(毫秒)
 */
uint32_t app_time_ms_get(void);

/**
 * @brief 将毫秒转换为滴答数
 * @param ms 毫秒时间
 * @return uint32_t 对应的滴答数
 * @note 如果输入为负数，返回0xffffffff
 */
uint32_t app_tick_get_from_ms(int32_t ms);

/**
 * @brief 计算时间差
 * @param now_ms 当前时间戳(ms)
 * @param prev_ms 之前的时间戳(ms)
 * @return 时间差(ms)
 * @note 支持时间戳溢出的情况
 */
uint32_t app_time_ms_diff(uint32_t now_ms, uint32_t prev_ms);

/**
 * @brief uint32_t绝对值差值计算
 * @return a和b的差值绝对值
 */
uint32_t app_compare_uint32_diff(int32_t a, int32_t b);
/**
 * @brief int32_t绝对值差值计算
 * @return a和b的差值绝对值
 */
uint32_t app_compare_int32_diff(int32_t a, int32_t b);

/**
 * @brief 微秒级延时
 * @param us 延时时间(us)
 */
void app_delay_us(uint32_t us);

/**
 * @brief 毫秒级延时
 * @param ms 延时时间(ms)
 */
void app_delay_ms(uint32_t ms);

/**
 * @brief 分割字符串为多个子串
 * @param input 输入字符串（将被修改）
 * @param tokens 存储分割结果的指针数组
 * @param max_tokens 最大分割数量
 * @return 实际分割出的子串数量
 */
uint32_t app_string_split(char *input, char **tokens, uint32_t max_tokens);


const char* file_name_get(const char* p_fpath);

/**
 * @brief CRC-32 计算（初始值固定为 0xFFFFFFFF）
 * @param p_data   输入数据指针
 * @param data_len 数据长度（字节）
 * @return         CRC-32 校验值
 */
uint32_t app_crc32(uint8_t const * p_data, uint32_t data_len);

/**
 * @brief CRC-32 扩展计算（支持分段计算）
 * @param p_data   输入数据指针
 * @param data_len 数据长度（字节）
 * @param crc      CRC 初始值（首次调用传 0xFFFFFFFF，后续传上次返回值）
 * @return         本次数据的 CRC-32 校验值
 */
uint32_t app_crc32_ext(uint8_t const * p_data, uint32_t data_len, uint32_t crc);


#ifdef __cplusplus
}
#endif

#endif /* __APP_COMMON_H__ */

/************************ END OF FILE *****************************************/
