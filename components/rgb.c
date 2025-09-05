#include "rgb.h"

// 静态变量
static volatile int rgb_signal_received = 0;
static volatile int rgb_running = 1;

// 信号处理函数
void rgb_signal_handler(int signal)
{
    rgb_signal_received = signal;
    printf("\nRGB组件接收到信号 %d\n", signal);
    
    switch (signal)
    {
        case SIGINT:  // Ctrl+C - 设置停止标志
            printf("RGB组件: 接收到退出信号，准备返回主菜单...\n");
            rgb_running = 0;
            break;
        default:
            printf("RGB组件: 未处理的信号 %d\n", signal);
            break;
    }
}

// 设置信号处理器
void rgb_setup_signal_handlers(void)
{
    signal(SIGINT, rgb_signal_handler);
    printf("RGB信号处理器设置完成 (仅SIGINT)\n");
}

// 清理函数
void rgb_cleanup(void)
{
    printf("RGB组件清理中...\n");
    set_rgb(0, 0, 0); // 关闭所有LED
    rgb_running = 1; // 重置运行状态以便下次使用
    printf("RGB组件清理完成\n");
}

// 获取运行状态
int rgb_is_running(void)
{
    return rgb_running;
}

void rgb_init(void)
{
    // 不再重复初始化wiringPi，因为web_main.c中已经初始化过了
    printf("RGB初始化开始...\n");
    
    // 设置引脚为输出模式
    printf("设置RGB引脚为输出模式...\n");
    pinMode(R, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(B, OUTPUT);
    printf("引脚模式设置完成: R=%d, G=%d, B=%d\n", R, G, B);

    // 初始状态：所有LED关闭
    printf("设置初始状态（全部关闭）...\n");
    digitalWrite(R, 0);
    digitalWrite(G, 0);
    digitalWrite(B, 0);

    printf("RGB组件初始化完成 (引脚 R:%d G:%d B:%d)\n\n", R, G, B);
}

void set_rgb(int red, int green, int blue)
{
    printf("设置RGB: R=%d, G=%d, B=%d\n", red, green, blue);
    digitalWrite(R, red);
    digitalWrite(G, green);
    digitalWrite(B, blue);
    printf("GPIO写入完成\n");
}

// 新增：rgb_set_color函数（Web API使用）
void rgb_set_color(int red, int green, int blue)
{
    printf("rgb_set_color调用: R=%d, G=%d, B=%d\n", red, green, blue);
    set_rgb(red, green, blue);
}

void rgb_sequence(void)
{
    rgb_running = 1; // 重置运行标志
    while (rgb_running)
    {
        // 红色亮1秒
        set_rgb(1, 0, 0);
        sleep(1);
        if (!rgb_running) break;

        // 绿色亮1秒（同时红色保持亮）
        set_rgb(1, 1, 0);
        sleep(1);
        if (!rgb_running) break;

        // 绿色关闭，蓝色亮1秒（红色保持亮）
        set_rgb(1, 0, 1);
        sleep(1);
        if (!rgb_running) break;

        // 红色关闭，蓝色关闭，绿色亮1秒
        set_rgb(0, 1, 0);
        sleep(1);
        if (!rgb_running) break;

        // 蓝色亮1秒（绿色保持亮）
        set_rgb(0, 1, 1);
        sleep(1);
        if (!rgb_running) break;

        // 绿色关闭1秒
        set_rgb(0, 0, 1);
        sleep(1);
        if (!rgb_running) break;

        // 绿色亮，红色亮1秒（蓝色保持亮）
        set_rgb(1, 1, 1);
        sleep(1);
    }
    
    // 退出时清理
    rgb_cleanup();
}
