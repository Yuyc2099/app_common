/*******************************************************************************
File name   : app_error.h
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-09-22
Description : 
History     : 
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_ERROR_H__
#define __APP_ERROR_H__

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported define -----------------------------------------------------------*/

#if 1
#define APP_EOK                         0               /**< There is no error */
#define APP_ERROR                       1               /**< A generic/unknown error happens */
#define APP_ETIMEOUT                    2               /**< Timed out */
#define APP_EFULL                       3               /**< The resource is full */
#define APP_EEMPTY                      4               /**< The resource is empty */
#define APP_ENOMEM                      5               /**< No memory */
#define APP_ENOSYS                      6               /**< Function not implemented */
#define APP_EBUSY                       7               /**< Busy */
#define APP_EIO                         8               /**< IO error */
#define APP_EINTR                       9               /**< Interrupted system call */
#define APP_EINVAL                      10              /**< Invalid argument */
#define APP_ENOENT                      11              /**< No entry */
#define APP_ENOSPC                      12              /**< No space left */
#define APP_EPERM                       13              /**< Operation not permitted */
#define APP_ETRAP                       14              /**< Trap event */
#define APP_EFAULT                      15              /**< Bad address */
#define APP_ENOBUFS                     16              /**< No buffer space is available */
#define APP_ESCHEDISR                   17              /**< scheduler failure in isr context */
#define APP_ESCHEDLOCKED                18              /**< scheduler failure in critical region */
#endif

/* Exported macro ------------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Private moudle variables --------------------------------------------------*/

/* Private moudle functions --------------------------------------------------*/

/* Private moudle end_flag ---------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __APP_ERROR_H__ */

/************************ END OF FILE *****************************************/
