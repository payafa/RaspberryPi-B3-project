#include "beep.h"

// 静态变量，只在当前文件可见
static int beep_current_state = 0;
static volatile int signal_received = 0;
static volatile int beep_running = 1;

// 信号处理函数
void beep_signal_handler(int signal)
{
    signal_received = signal;
    printf("\n蜂鸣器组件接收到信号 %d\n", signal);
    
    switch (signal)
    {
        case SIGINT:  // Ctrl+C - 设置停止标志
            printf("蜂鸣器组件: 接收到退出信号，准备返回主菜单...\n");
            beep_running = 0;
            break;
        default:
            printf("蜂鸣器组件: 未处理的信号 %d\n", signal);
            break;
    }
}

// 设置信号处理器
void beep_setup_signal_handlers(void)
{
    signal(SIGINT, beep_signal_handler);
    printf("蜂鸣器信号处理器设置完成 (仅SIGINT)\n");
}

// 初始化蜂鸣器
void beep_init(void)
{
    pinMode(BEEP_PIN, OUTPUT);
    digitalWrite(BEEP_PIN, 0);
    beep_current_state = 0;
    printf("蜂鸣器初始化完成 (引脚 %d)\n", BEEP_PIN);
}

// 打开蜂鸣器
void beep_on(void)
{
    digitalWrite(BEEP_PIN, 1);
    beep_current_state = 1;
    printf("蜂鸣器: 开启\n");
}

// 关闭蜂鸣器
void beep_off(void)
{
    digitalWrite(BEEP_PIN, 0);
    beep_current_state = 0;
    printf("蜂鸣器: 关闭\n");
}

// 切换蜂鸣器状态
void beep_toggle(void)
{
    beep_current_state = !beep_current_state;
    digitalWrite(BEEP_PIN, beep_current_state);
    printf("蜂鸣器: %s\n", beep_current_state ? "开启" : "关闭");
}

// 设置蜂鸣器状态
void beep_set_state(int state)
{
    if (state)
    {
        beep_on();
    }
    else
    {
        beep_off();
    }
}

// 获取蜂鸣器当前状态
int beep_get_state(void)
{
    return beep_current_state;
}

// 获取运行状态
int beep_is_running(void)
{
    return beep_running;
}

// 清理函数
void beep_cleanup(void)
{
    printf("蜂鸣器组件清理中...\n");
    beep_off();
    beep_running = 1; // 重置运行状态以便下次使用
    printf("蜂鸣器组件清理完成\n");
}
