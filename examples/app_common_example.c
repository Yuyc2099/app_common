/*******************************************************************************
File name   : app_common_example.c
Description : app_common module usage example
*******************************************************************************/

#define DEBUG_LEVEL                     DBG_LV_INFO
#define DEBUG_TAG                       "example"

#include "app_config.h"

static uint32_t s_example_count;
static void *s_example_parameter = &s_example_count;

static void example_event_handler(uint32_t evtmod, uint32_t event_id, const void *data)
{
    if (evtmod != APP_EVTMOD_EXAMPLE || event_id != APP_EVTID_EXAMPLE_START) {
        return;
    }

    if (data != NULL) {
        print_info("event count: %u", *(const uint32_t *)data);
    }
}

static uint32_t example_app_init(void)
{
    app_print_enable(true);

#if USE_APP_EVENT
    return app_event_subscribe(example_event_handler, APP_EVTMOD_EXAMPLE);
#else
    return APP_EOK;
#endif
}
APP_APPLICATION_INIT(example_app_init);

static uint32_t example_thread(void *parameter)
{
    static uint32_t last_time_ms;
    uint32_t now_ms = app_time_ms_get();

    if (app_time_ms_diff(now_ms, last_time_ms) >= 5000U) {
        last_time_ms = now_ms;
        print_info("thread count: %u", *(uint32_t *)parameter);
    }

    return APP_EOK;
}
APP_THREAD_REGISTER(example_thread, &s_example_parameter, APP_THREAD_MIDDLE_LEVEL);

static uint32_t example_timer(void *parameter)
{
    uint32_t *count = (uint32_t *)parameter;

    (*count)++;
#if USE_APP_EVENT
    app_event_notify_data(APP_EVTMOD_EXAMPLE, APP_EVTID_EXAMPLE_START, count);
#endif
    return APP_EOK;
}
APP_TIMER_REGISTER(example_timer, &s_example_parameter, 1000U, true, 1000U);

static uint32_t example_shell(char *argv[], uint32_t argc)
{
    (void)argv;
    (void)argc;

    print_info("count: %u", s_example_count);
    return APP_EOK;
}
APP_SHELL_REGISTER("example", example_shell);

/************************ END OF FILE *****************************************/
