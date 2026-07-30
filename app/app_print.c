/*******************************************************************************
File name   : app_print.c
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-11-06
Description : 
History     : 

*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "app_print.h"

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
bool g_print_enable = true;

/* Private moudle variables --------------------------------------------------*/

/* Function declaration ------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Private moudle functions --------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
// @brief 获取当前时间
const char* app_print_time_get(void)
{
    char time_fmt[16];
    snprintf(time_fmt, sizeof(time_fmt), "[%d]", app_time_ms_get());

    static char buff[16];
    memcpy(buff, time_fmt, sizeof(time_fmt));
    return buff;
}

/**
 * @brief 以HEX格式将数据打印，并在行尾显示ACSII字符，每行打印长度为16,由number控制
 * @param name: 输出数据的名称前缀
 * @param buff: 需要输出的数据
 * @param size: 需要输出数据的长度
*/
void app_hex_print(uint8_t *name, uint8_t *buff, uint32_t size)
{
    #define CHAR_PRINTF(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')
    int i, j;
    int number = 16;
    
    kprintf("[HEX] %s: \r\n", name);
    for (i = 0; i < size; i += number)
    {
        kprintf("%04X: ", i);

        for (j = 0; j < number; j++)
        {
            if (j % 8 == 0)
            {
                kprintf(" ");
            }

            if (i + j < size)
            {
                kprintf("%02X ", buff[i + j]);
            }
            else
            {
                kprintf("   ");
            }
        }
        kprintf(" ");

        for (j = 0; j < number; j++)
        {
            if (i + j < size)
            {
                kprintf("%c", CHAR_PRINTF(buff[i + j]) ? buff[i + j] : '.');
            }
        }
        kprintf("\r\n");
    }
}


/************************ END OF FILE *****************************************/
