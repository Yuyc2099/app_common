/*******************************************************************************
File name   : app_compiler.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-09-17
Description : 
History     : 
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_COMPILER_H__
#define __APP_COMPILER_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

#if defined(__ARMCC_VERSION)            /* ARM Compiler */
#define app_section(x)                  __attribute__((section(x)))
#define app_used                        __attribute__((used))
#define app_align(n)                    __attribute__((aligned(n)))
#if (__ARMCC_VERSION >= 6000000U)
#define app_packed(declare)             declare __attribute__((packed))
#define app_return_address(n)           (uint32_t)__builtin_return_address(0)
#else
#define app_packed(declare)             __packed declare
#define app_return_address(n)           (uint32_t)__return_address()
#endif
#define app_weak                        __attribute__((weak))
#define app_typeof                      __typeof
#define app_noreturn
#define app_inline                      static __inline
#define app_always_inline               app_inline
#elif defined (__IAR_SYSTEMS_ICC__)     /* for IAR Compiler */
#define app_section(x)                  @ x
#define app_used                        __root
#define PRAGMA(x)                       _Pragma(#x)
#define app_align(n)                    PRAGMA(data_alignment=n)
#define app_packed(declare)             declare
#define app_weak                        __weak
#define app_typeof                      __typeof
#define app_noreturn
#define app_inline                      static inline
#define app_always_inline               app_inline
#elif defined (__GNUC__)                /* GNU GCC Compiler */
#define __APP_STRINGIFY(x...)           #x
#define APP_STRINGIFY(x...)             __APP_STRINGIFY(x)
#define app_section(x)                  __attribute__((section(x)))
#define app_used                        __attribute__((used))
#define app_align(n)                    __attribute__((aligned(n)))
#define app_packed(declare)             declare __attribute__((packed))
#define app_weak                        __attribute__((weak))
#define app_typeof                      __typeof__
#define app_noreturn                    __attribute__ ((noreturn))
#define app_inline                      static __inline
#define app_always_inline               static inline __attribute__((always_inline))
#define app_return_address(n)           (uint32_t)__builtin_return_address(0)
#elif defined (__ADSPBLACKFIN__)        /* for VisualDSP++ Compiler */
#define app_section(x)                  __attribute__((section(x)))
#define app_used                        __attribute__((used))
#define app_align(n)                    __attribute__((aligned(n)))
#define app_packed(declare)             declare
#define app_weak                        __attribute__((weak))
#define app_typeof                      typeof
#define app_noreturn
#define app_inline                      static inline
#define app_always_inline               app_inline
#elif defined (_MSC_VER)                /* for Visual Studio Compiler */
#define app_section(x)
#define app_used    
#define app_align(n)                    __declspec(align(n))
#define app_packed(declare)             __pragma(pack(push, 1)) declare __pragma(pack(pop))
#define app_weak
#define app_typeof                      typeof
#define app_noreturn
#define app_inline                      static __inline
#define app_always_inline               app_inline
#elif defined (__TI_COMPILER_VERSION__) /* for TI CCS Compiler */
/**
 * The way that TI compiler set section is different from other(at least
 * GCC and MDK) compilers. See ARM Optimizing C/C++ Compiler 5.9.3 for more
 * details.
 */
#define app_section(x)                  __attribute__((section(x)))
#ifdef __TI_EABI__
#define app_used                        __attribute__((retain)) __attribute__((used))
#else
#define app_used                        __attribute__((used))
#endif
#define PRAGMA(x)                       _Pragma(#x)
#define app_align(n)                    __attribute__((aligned(n)))
#define app_packed(declare)             declare __attribute__((packed))
#ifdef __TI_EABI__
#define app_weak                        __attribute__((weak))
#else
#define app_weak
#endif
#define app_typeof                      typeof
#define app_noreturn
#define app_inline                      static inline
#define app_always_inline               app_inline
#elif defined (__TASKING__)             /* for TASKING Compiler */
#define app_section(x)                  __attribute__((section(x)))
#define app_used                        __attribute__((used, protect))
#define PRAGMA(x)                       _Pragma(#x)
#define app_align(n)                    __attribute__((__align(n)))
#define app_packed(declare)             declare __packed__
#define app_weak                        __attribute__((weak))
#define app_typeof                      typeof
#define app_noreturn
#define app_inline                      static inline
#define app_always_inline               app_inline
#else                                   /* Unkown Compiler */
    #error not supported tool chain
#endif /* __ARMCC_VERSION */

#ifndef app_return_address
#define app_return_address(n)           (0)
#endif

#endif /* __APP_COMPILER_H__ */

/************************ END OF FILE *****************************************/
