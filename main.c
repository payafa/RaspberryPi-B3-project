#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <wiringPi.h>
#include "components/beep.h"
#include "components/botton.h"
#include "components/clock.h"
#include "components/rgb.h"
#include "combo/alarm_clock.h"
#include "combo/stopwatch.h"
#include "combo/rgb_control.h"

// 函数声明
void show_main_menu(void);
void show_single_component_menu(void);
void show_combo_component_menu(void);
void test_beep(void);
void test_button(void);
void test_clock(void);
void test_rgb(void);
void clear_screen(void);
void wait_for_input(void);

int main(void) {
    int choice;
    
    // 初始化 wiringPi
    if (wiringPiSetupGpio() == -1) {
        printf("初始化 wiringPi 失败!\n");
        return 1;
    }
    
    printf("=== 树莓派B3项目控制系统 ===\n");
    printf("系统初始化中...\n");
    
    // 初始化各个组件
    beep_init();
    botton_init();
    tm1637_init();
    rgb_init();
    
    printf("系统初始化完成!\n");
    sleep(1);
    
    while (1) {
        clear_screen();
        show_main_menu();
        
        printf("请选择功能 (1-3): ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                show_single_component_menu();
                break;
            case 2:
                show_combo_component_menu();
                break;
            case 3:
                printf("感谢使用！再见！\n");
                // 清理资源
                beep_off();
                rgb_cleanup();
                clock_cleanup();
                exit(0);
                break;
            default:
                printf("无效选择，请重新输入！\n");
                wait_for_input();
                break;
        }
    }
    
    return 0;
}

void show_main_menu(void) {
    printf("\n");
    printf("╔══════════════════════════════════════╗\n");
    printf("║          树莓派B3项目控制系统        ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║  1. 单个元件功能测试                 ║\n");
    printf("║  2. 组合元件功能演示                 ║\n");
    printf("║  3. 退出系统                         ║\n");
    printf("╚══════════════════════════════════════╝\n");
    printf("\n");
}

void show_single_component_menu(void) {
    int choice;
    
    while (1) {
        clear_screen();
        printf("\n");
        printf("╔══════════════════════════════════════╗\n");
        printf("║          单个元件功能测试            ║\n");
        printf("╠══════════════════════════════════════╣\n");
        printf("║  1. 蜂鸣器测试                       ║\n");
        printf("║  2. 按键测试                         ║\n");
        printf("║  3. 时钟显示测试                     ║\n");
        printf("║  4. RGB LED测试                      ║\n");
        printf("║  5. 返回主菜单                       ║\n");
        printf("╚══════════════════════════════════════╝\n");
        printf("\n");
        
        printf("请选择测试项目 (1-5): ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                test_beep();
                break;
            case 2:
                test_button();
                break;
            case 3:
                test_clock();
                break;
            case 4:
                test_rgb();
                break;
            case 5:
                return; // 返回主菜单
            default:
                printf("无效选择，请重新输入！\n");
                wait_for_input();
                break;
        }
    }
}

void show_combo_component_menu(void) {
    int choice;
    
    while (1) {
        clear_screen();
        printf("\n");
        printf("╔══════════════════════════════════════╗\n");
        printf("║          组合元件功能演示            ║\n");
        printf("╠══════════════════════════════════════╣\n");
        printf("║  1. 闹钟功能                         ║\n");
        printf("║  2. 秒表功能                         ║\n");
        printf("║  3. 按键控制RGB灯颜色切换            ║\n");
        printf("║  4. 温度显示功能                     ║\n");
        printf("║  5. 系统状态指示                     ║\n");
        printf("║  6. 返回主菜单                       ║\n");
        printf("╚══════════════════════════════════════╝\n");
        printf("\n");
        
        printf("请选择演示项目 (1-6): ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                alarm_clock_function();
                break;
            case 2:
                stopwatch_function();
                break;
            case 3:
                rgb_button_control();
                break;
            case 4:
                temperature_display_function();
                break;
            case 5:
                system_status_indicator();
                break;
            case 6:
                return; // 返回主菜单
            default:
                printf("无效选择，请重新输入！\n");
                wait_for_input();
                break;
        }
    }
}

void test_beep(void) {
    int choice;
    
    clear_screen();
    printf("\n=== 蜂鸣器测试 ===\n");
    printf("1. 开启蜂鸣器\n");
    printf("2. 关闭蜂鸣器\n");
    printf("3. 蜂鸣器切换\n");
    printf("4. 返回\n");
    printf("请选择 (1-4): ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            printf("开启蜂鸣器...\n");
            beep_on();
            printf("蜂鸣器已开启，按回车键关闭并返回\n");
            getchar(); // 清除输入缓冲
            getchar(); // 等待用户输入
            beep_off();
            break;
        case 2:
            printf("关闭蜂鸣器...\n");
            beep_off();
            wait_for_input();
            break;
        case 3:
            printf("蜂鸣器切换测试 (按回车停止)...\n");
            getchar(); // 清除输入缓冲
            for (int i = 0; i < 5; i++) {
                beep_toggle();
                usleep(500000); // 0.5秒
            }
            beep_off();
            printf("测试完成\n");
            wait_for_input();
            break;
        case 4:
            return;
        default:
            printf("无效选择！\n");
            wait_for_input();
            break;
    }
}

void test_button(void) {
    clear_screen();
    printf("\n=== 按键测试 ===\n");
    printf("请按下按键进行测试 (按住Ctrl+C退出测试)\n");
    printf("按键状态监测中...\n\n");
    
    // 设置信号处理
    beep_setup_signal_handlers();
    
    while (1) {
        if (botton_is_pressed()) {
            printf("按键被按下！\n");
            beep_on(); // 按键按下时蜂鸣器响
        }
        
        if (botton_is_released()) {
            printf("按键被释放！\n");
            beep_off(); // 按键释放时蜂鸣器停
        }
        
        usleep(50000); // 50ms延时
    }
}

void test_clock(void) {
    int choice;
    char text[100];
    
    clear_screen();
    printf("\n=== 时钟显示测试 ===\n");
    printf("1. 显示当前时间\n");
    printf("2. 显示自定义文本\n");
    printf("3. 滚动显示文本\n");
    printf("4. 返回\n");
    printf("请选择 (1-4): ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            printf("显示当前时间 (按回车停止)...\n");
            clock_setup_signal_handlers();
            getchar(); // 清除输入缓冲
            clock_display(); // 这个函数应该包含循环显示时间
            break;
        case 2:
            printf("请输入要显示的文本 (最多4个字符): ");
            scanf("%s", text);
            printf("显示文本: %s (按回车返回)...\n", text);
            text_display(text);
            getchar(); // 清除输入缓冲
            getchar(); // 等待用户输入
            break;
        case 3:
            printf("请输入要滚动显示的文本: ");
            getchar(); // 清除输入缓冲
            fgets(text, sizeof(text), stdin);
            text[strlen(text)-1] = '\0'; // 移除换行符
            printf("滚动显示文本: %s (按回车停止)...\n", text);
            roll_display(text, strlen(text));
            break;
        case 4:
            return;
        default:
            printf("无效选择！\n");
            wait_for_input();
            break;
    }
}

void test_rgb(void) {
    int choice;
    int r, g, b;
    
    clear_screen();
    printf("\n=== RGB LED测试 ===\n");
    printf("1. RGB序列演示\n");
    printf("2. 自定义颜色\n");
    printf("3. 关闭RGB灯\n");
    printf("4. 返回\n");
    printf("请选择 (1-4): ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            printf("RGB序列演示 (按回车停止)...\n");
            rgb_setup_signal_handlers();
            getchar(); // 清除输入缓冲
            rgb_sequence(); // 这个函数应该包含序列演示
            break;
        case 2:
            printf("请输入RGB值 (0-1):\n");
            printf("红色 (0/1): ");
            scanf("%d", &r);
            printf("绿色 (0/1): ");
            scanf("%d", &g);
            printf("蓝色 (0/1): ");
            scanf("%d", &b);
            printf("设置RGB为 (%d,%d,%d) (按回车返回)...\n", r, g, b);
            set_rgb(r, g, b);
            getchar(); // 清除输入缓冲
            getchar(); // 等待用户输入
            break;
        case 3:
            printf("关闭RGB灯...\n");
            set_rgb(0, 0, 0);
            wait_for_input();
            break;
        case 4:
            return;
        default:
            printf("无效选择！\n");
            wait_for_input();
            break;
    }
}

void clear_screen(void) {
    system("clear"); // Linux/Unix系统
    // system("cls"); // Windows系统 - 如果在Windows上运行请取消注释这行并注释上面一行
}

void wait_for_input(void) {
    printf("按回车键继续...");
    getchar(); // 清除输入缓冲
    getchar(); // 等待用户输入
}
