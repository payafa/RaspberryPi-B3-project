#include <stdio.h>
#include <unistd.h>
#include "botton.h"
#include "clock.h"
#include "beep.h"
#include "rgb.h"

// 菜单显示
void display_menu(void)
{
    printf("\n=== 元件控制系统 ===\n");
    printf("1. 按钮状态检测\n");
    printf("2. 时钟显示测试\n");
    printf("3. RGB灯测试\n");
    printf("4. 蜂鸣器测试\n");
    printf("5. 运行演示模式\n");
    printf("0. 退出\n");
    printf("请选择: ");
}

// 演示模式
void demo_mode(void)
{
    printf("\n开始演示模式...\n");

    // RGB演示
    printf("RGB灯演示...\n");
    rgb_sequence(); // 使用头文件中的rgb_sequence函数

    // 时钟演示
    printf("时钟显示演示...\n");
    char test_data[] = "1234";
    data_display(test_data);
    sleep(2);

    // 蜂鸣器演示
    printf("蜂鸣器演示...\n");
    beep_on();
    sleep(1);
    beep_off();

    printf("演示结束\n");
}

int main(void)
{
    // 初始化所有元件
    printf("初始化元件...\n");
    wiringPiSetupGpio(); // 需要添加GPIO初始化

    botton_init();
    beep_init();
    tm1637_init(); // 使用头文件中的tm1637_init函数
    rgb_init();

    int choice;

    while (1)
    {
        display_menu();
        if (scanf("%d", &choice) != 1)
        {
            // 清除输入缓冲区
            while (getchar() != '\n')
                ;
            printf("请输入有效的数字\n");
            continue;
        }

        switch (choice)
        {
        case 0:
            printf("退出程序\n");
            // 关闭所有元件
            set_rgb(0, 0, 0); // 关闭RGB
            beep_off();       // 关闭蜂鸣器
            return 0;

        case 1:
            printf("按钮状态检测中...按Ctrl+C退出\n");
            while (1)
            {
                if (botton_is_pressed())
                {
                    printf("按钮按下\n");
                    beep_on(); // 按下时蜂鸣器响
                }
                else if (botton_is_released())
                {
                    printf("按钮释放\n");
                    beep_off(); // 释放时蜂鸣器关闭
                }
                usleep(100000); // 100ms延迟
            }
            break;

        case 2:
            printf("时钟显示测试\n");
            // 显示测试时间 12:34
            clock_display();
            sleep(3);
            break;

        case 3:
            printf("RGB灯测试\n");
            // 测试不同颜色
            set_rgb(255, 0, 0); // 红色
            sleep(1);
            set_rgb(0, 255, 0); // 绿色
            sleep(1);
            set_rgb(0, 0, 255); // 蓝色
            sleep(1);
            set_rgb(0, 0, 0); // 关闭
            break;

        case 4:
            printf("蜂鸣器测试\n");
            beep_on();
            sleep(1);
            beep_off();
            break;

        case 5:
            demo_mode();
            break;

        default:
            printf("无效选择\n");
        }
    }

    return 0;
}
