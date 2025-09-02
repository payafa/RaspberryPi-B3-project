#include "rgb.h"

void rgb_init(void)
{
    if (wiringPiSetupGpio() < 0)
    {
        perror("启动树莓派GPIO失败");
        exit(1);
    }

    // 设置引脚为输出模式
    pinMode(R, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(B, OUTPUT);

    // 初始状态：所有LED关闭
    digitalWrite(R, 0);
    digitalWrite(G, 0);
    digitalWrite(B, 0);
}

void set_rgb(int red, int green, int blue)
{
    digitalWrite(R, red);
    digitalWrite(G, green);
    digitalWrite(B, blue);
}

void rgb_sequence(void)
{
    while (1)
    {
        // 红色亮1秒
        set_rgb(1, 0, 0);
        sleep(1);

        // 绿色亮1秒（同时红色保持亮）
        set_rgb(1, 1, 0);
        sleep(1);

        // 绿色关闭，蓝色亮1秒（红色保持亮）
        set_rgb(1, 0, 1);
        sleep(1);

        // 红色关闭，蓝色关闭，绿色亮1秒
        set_rgb(0, 1, 0);
        sleep(1);

        // 蓝色亮1秒（绿色保持亮）
        set_rgb(0, 1, 1);
        sleep(1);

        // 绿色关闭1秒
        set_rgb(0, 0, 1);
        sleep(1);

        // 绿色亮，红色亮1秒（蓝色保持亮）
        set_rgb(1, 1, 1);
        sleep(1);
    }
}
