/*******************************************************************************
Copyright   : 深圳市爱都科技有限公司
File name   : app_save_data.c
Author      : Yu You Cheng
Version     : V1.0
Date        : 2025-07-30
Description : 微信支付数据的保存，微信数据使用10多个KEY，并且存在数据加密的情况，所以独立出来
History     : 修改记录，每条记录应包括修改日期、修改者及修改内容简述
              1. Date:
                  Author:
                  Modification:

*******************************************************************************/

/* Includes --------------------------------------------------------------------*/
#define DEBUG_LEVEL DBG_LV_DEBUG
#define DEBUG_TAG   "save_data"

#include "app_config.h"
#if USE_APP_SAVE_DATA
#include "app_save_data.h"

/* Private define --------------------------------------------------------------*/
#define APP_SAVE_DATA_MAGIC_NUMBER      (0x38FD)
#define APP_SAVE_DATA_KEY_INVALID       (0xFF)

#define APP_SAVE_DATA_HEAD_SIZE         (sizeof(app_save_data_head_t))

/* Private macro --------------------------------------------------------------*/

/* Private typedef ------------------------------------------------------------*/
typedef struct
{
    uint16_t magic_number;
    uint16_t check_number;
    uint16_t length;
    uint16_t crc;
} app_save_data_head_t;

typedef struct
{
    uint32_t address;
    uint16_t data_size;
    uint16_t map_size;
    uint16_t map_count;
    uint16_t init : 1;
    uint16_t reserve : 15;
    app_data_size_map_t **p_map;
} app_save_data_addr_t;

/* Private variables -----------------------------------------------------------*/
static app_save_data_addr_t s_app_data = {0};

/* Private functions -----------------------------------------------------------*/
static uint8_t get_index_by_key(uint16_t key)
{
    if (!s_app_data.init || s_app_data.p_map == NULL)
    {
        return APP_SAVE_DATA_KEY_INVALID;
    }
    for(int i = 0 ; i < s_app_data.map_count ; i ++)
    {
        if(s_app_data.p_map[i]->key == key)
        {
            return i;
        }
    }
    print_debug("%s APP_SAVE_DATA_KEY_INVALID", __func__);
    return APP_SAVE_DATA_KEY_INVALID;
}
/* Exported functions ---------------------------------------------------------*/
app_weak app_data_size_map_t **app_save_data_get_map(uint16_t *p_map_count)
{
    print_error("weak function: %s!!!", __func__);
    *p_map_count = 0;
    return NULL;
}

app_weak uint32_t app_flash_save_data_address_get(uint32_t *p_address, uint16_t *p_data_size)
{
    print_error("weak function: %s!!!", __func__);
    *p_address = 0;
    *p_data_size = 0;
    return APP_EEMPTY;
}

app_weak uint32_t app_flash_read(uint32_t address, void *p_data, uint16_t *p_data_size)
{
    print_error("weak function: %s!!!", __func__);
    *p_data_size = 0;
    return APP_EEMPTY;
}

app_weak uint32_t app_flash_write(uint32_t address, void *p_data, uint16_t data_size)
{
    print_error("weak function: %s!!!", __func__);
    return APP_EEMPTY;
}

/**
 * @brief   微信支付保存数据初始化
 * @retval  APP_EXXX
*/
uint32_t app_save_data_init(void)
{
    memset(&s_app_data, 0, sizeof(s_app_data));
    
    s_app_data.p_map = app_save_data_get_map(&s_app_data.map_count);
    uint32_t ret = app_flash_save_data_address_get(&s_app_data.address, &s_app_data.data_size);
    APP_ASSERT(s_app_data.p_map != NULL && s_app_data.map_count != 0 && ret == APP_EOK);

    uint32_t offset = 0;
    for(int i = 0 ; i < s_app_data.map_count; i ++)
    {
        s_app_data.p_map[i]->block_offset = offset;
        offset += s_app_data.p_map[i]->data_size;
    }
    s_app_data.map_size = offset;
    APP_ASSERT(s_app_data.map_size <= s_app_data.data_size);
    s_app_data.init = true;
    print_debug("%s data_size=%d, map_size=%d", __func__, s_app_data.data_size, s_app_data.map_size);
    return APP_EOK;
}

/**
 * @brief   微信支付保存数据读取
 * @note    读取所有的数据，返回字节数为实际读取的长度
 * @param[int-out] *read_size 传入data的可使用字节数，返回为实际读取到的数据大小
 * @retval APP_EXXX
*/
uint32_t app_save_data_read(uint16_t index, void *data, uint16_t *data_size)
{
    *data_size = 0;
    if (!s_app_data.init || s_app_data.p_map == NULL)
    {
        return APP_ERROR;
    }
    uint8_t arr_index = get_index_by_key(index);
    if(arr_index == APP_SAVE_DATA_KEY_INVALID)
    {
        return APP_EINVAL;
    }

    app_save_data_head_t head = {0};
    uint16_t head_size = APP_SAVE_DATA_HEAD_SIZE;
    app_flash_read(s_app_data.p_map[arr_index]->block_offset, &head, &head_size);
    if (head.magic_number != APP_SAVE_DATA_MAGIC_NUMBER || head.check_number != index \
        || head.length > (UINT16_MAX - APP_SAVE_DATA_HEAD_SIZE))
    {
        print_debug("%s magic=0x%x, check_number=%d, index=%d", __func__, head.magic_number, head.check_number, index);
        return APP_EINVAL;
    }

    *data_size = head.length;
    app_flash_read(s_app_data.p_map[arr_index]->block_offset + APP_SAVE_DATA_HEAD_SIZE, data, data_size);
    return APP_EOK;
}

/**
 * @brief   微信支付保存数据写入
 * @retval  APP_EXXX
*/
uint32_t app_save_data_write(uint16_t index, void *data, uint16_t data_size)
{
    if (!s_app_data.init || s_app_data.p_map == NULL)
    {
        return APP_ERROR;
    }
    uint8_t arr_index = get_index_by_key(index);
    if(arr_index == APP_SAVE_DATA_KEY_INVALID)
    {
        return APP_EINVAL;
    }

    if(data_size + APP_SAVE_DATA_HEAD_SIZE > s_app_data.p_map[arr_index]->data_size)
    {
        APP_ASSERT(0);
        return APP_ENOSPC;
    }

    app_save_data_head_t head = {0};
    head.magic_number = APP_SAVE_DATA_MAGIC_NUMBER;
    head.check_number = index;
    head.length = data_size;
    head.crc = 0xffff; //TODO
    app_flash_write(s_app_data.p_map[arr_index]->block_offset + APP_SAVE_DATA_HEAD_SIZE, (uint8_t *)data, data_size);
    app_flash_write(s_app_data.p_map[arr_index]->block_offset, &head, APP_SAVE_DATA_HEAD_SIZE);
    return APP_EOK;
}

/**
 * @brief   微信支付保存数据清除
 * @retval  APP_EXXX
 */
uint32_t app_save_data_clear(uint16_t index)
{
    if (!s_app_data.init || s_app_data.p_map == NULL)
    {
        return APP_ERROR;
    }
    uint8_t arr_index = get_index_by_key(index);
    if(arr_index == APP_SAVE_DATA_KEY_INVALID)
    {
        return APP_EINVAL;
    }

    app_save_data_head_t head = {0};
    head.crc = 0xffff; //TODO
    app_flash_write(s_app_data.p_map[arr_index]->block_offset, (void *)&head, APP_SAVE_DATA_HEAD_SIZE);
    return APP_EOK;
}

/**
 * @brief 微信支付保存数据是否存在
 * @retval  APP_EXXX
*/
uint32_t app_save_data_exist(uint16_t index)
{
    uint8_t arr_index = get_index_by_key(index);
    if(arr_index == APP_SAVE_DATA_KEY_INVALID)
    {
        return APP_ERROR;
    }

    app_save_data_head_t head = {0};
    uint16_t head_size = APP_SAVE_DATA_HEAD_SIZE;
    app_flash_read(s_app_data.p_map[arr_index]->block_offset, &head, &head_size);
    uint32_t ret = APP_EINVAL;
    if(head.magic_number == APP_SAVE_DATA_MAGIC_NUMBER && head.check_number == index && head.length != 0)
    {
        ret = APP_EOK;
    }
    return ret;
}
#endif

/************************ END OF FILE *****************************************/
