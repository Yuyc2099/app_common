#define DEBUG_LEVEL DBG_LV_DEBUG
#define DEBUG_TAG   "RTT"
#include "app_config.h"

#if USE_SEGGER_RTT
/**
 * @brief  初始化RTT(Real-Time Transfer)功能
 * @note   该函数用于初始化SEGGER RTT功能，并打印初始化信息
 */
void RTT_Init(void)
{
    SEGGER_RTT_Init();
    print_debug("RTT_Init");
}

/**
 * @brief  从RTT缓冲区读取数据
 * @param  data: 存储读取数据的缓冲区指针
 * @param  len: 输入参数表示缓冲区大小，输出参数表示实际读取的数据长度
 * @retval APP_EOK: 读取成功
 * @retval APP_EEMPTY: 缓冲区无数据
 * @retval APP_ENOMEM: 缓冲区空间不足
 * @note   如果提供的缓冲区大小小于实际数据大小，会读取并丢弃所有数据
 */
uint32_t RTT_ReadData(char *data, uint32_t *len)
{
    uint32_t rlen = SEGGER_RTT_Read(RTT_DBG_PORT, data, *len);
    if (rlen == 0) {
        return APP_EEMPTY;
    }

    if (rlen < *len) {
        *len = rlen;
        return APP_EOK;
    } else {
        print_debug("len too small, %d <= %d\r\n", *len, rlen);
        while(1)
        {
            rlen = SEGGER_RTT_Read(RTT_DBG_PORT, data, *len);
            if (rlen == 0) {
                return APP_EFULL;
            }
        }
    }
}

#if USE_APP_SHELL
uint32_t app_shell_read_data(char *data, uint32_t *len)
{
    return RTT_ReadData(data, len);
}
#endif

#endif
