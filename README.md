# app_common

面向 ARM Cortex-M 裸机项目的轻量级公共组件库。

项目通过自定义 section 注册初始化函数、循环任务、软件定时器和 Shell 命令，减少模块之间的直接依赖。

## 主要功能

- 分级初始化
- 裸机协作式任务调度
- 软件定时器与事件通知
- Shell 和日志输出
- Flash 数据保存
- 断言、故障回溯及常用工具函数

## 目录说明

- `app/`：裸机公共组件
- `cm_backtrace/`：Cortex-M 故障回溯
- `SEGGER_RTT/`：SEGGER RTT
- `vofa/`：VOFA+ 数据发送
- `rtthread/`：可选兼容代码，裸机核心不依赖该目录
- `examples/`：宿主工程配置和模块使用示例

## 基本使用

1. 将需要的源文件加入工程。
2. 由宿主工程提供 `app_config.h`，配置模块开关和芯片相关接口。
3. 在链接脚本中保留所用模块的注册 section。
4. 在系统节拍中断中调用 `app_tick_increase()`。
5. 启动时调用 `app_init_process()`，主循环中调用 `app_thread_process()`。

```c
void SysTick_Handler(void)
{
    app_tick_increase();
}

int main(void)
{
    board_init();
    app_init_process();

    while (1) {
        app_thread_process();
    }
}
```

宿主配置可参考 [`examples/config/`](examples/config/README.md)，模块使用可参考
[`examples/app_common_example.c`](examples/app_common_example.c)。

第三方组件的许可证以各目录源文件中的声明为准。
