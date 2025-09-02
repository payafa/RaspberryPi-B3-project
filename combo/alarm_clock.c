#include "alarm_clock.h"

void alarm_clock_function(void) {
    int choice;
    int alarm_hour = 0, alarm_minute = 0;
    
    while (1) {
        system("clear");
        printf("\n=== 闹钟功能 ===\n");
        printf("当前闹钟设置: %02d:%02d\n", alarm_hour, alarm_minute);
        printf("1. 设置闹钟时间\n");
        printf("2. 启动闹钟监控\n");
        printf("3. 测试闹钟响铃\n");
        printf("4. 返回\n");
        printf("请选择 (1-4): ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                alarm_clock_set_time(&alarm_hour, &alarm_minute);
                break;
                
            case 2:
                alarm_clock_monitor(alarm_hour, alarm_minute);
                break;
                
            case 3:
                alarm_clock_test_ring();
                break;
                
            case 4:
                return;
                
            default:
                printf("无效选择！\n");
                printf("按回车键继续...");
                getchar();
                getchar();
                break;
        }
    }
}

void alarm_clock_set_time(int *hour, int *minute) {
    printf("请输入闹钟时间:\n");
    printf("小时 (0-23): ");
    scanf("%d", hour);
    printf("分钟 (0-59): ");
    scanf("%d", minute);
    
    // 验证输入
    if (*hour < 0 || *hour > 23) *hour = 0;
    if (*minute < 0 || *minute > 59) *minute = 0;
    
    printf("闹钟已设置为 %02d:%02d\n", *hour, *minute);
    printf("按回车键继续...");
    getchar();
    getchar();
}

void alarm_clock_monitor(int alarm_hour, int alarm_minute) {
    int current_hour, current_minute;
    time_t now;
    struct tm *timeinfo;
    
    printf("闹钟监控启动中... (按按钮停止监控)\n");
    printf("当前闹钟设置: %02d:%02d\n", alarm_hour, alarm_minute);
    
    while (!botton_is_pressed()) {
        time(&now);
        timeinfo = localtime(&now);
        current_hour = timeinfo->tm_hour;
        current_minute = timeinfo->tm_min;
        
        // 显示当前时间
        char time_str[5];
        sprintf(time_str, "%02d%02d", current_hour, current_minute);
        text_display(time_str);
        
        // 检查是否到达闹钟时间
        if (current_hour == alarm_hour && current_minute == alarm_minute) {
            printf("\n⏰ 闹钟响铃！\n");
            alarm_clock_ring();
            break;
        }
        
        sleep(30); // 每30秒检查一次
    }
    
    printf("闹钟监控已停止\n");
    printf("按回车键继续...");
    getchar();
    getchar();
}

void alarm_clock_ring(void) {
    // 闹钟响铃：蜂鸣器响 + RGB灯闪烁
    for (int i = 0; i < 10; i++) {
        beep_on();
        set_rgb(1, 0, 0); // 红色
        usleep(500000);   // 0.5秒
        beep_off();
        set_rgb(0, 0, 0); // 关闭
        usleep(500000);   // 0.5秒
        
        if (botton_is_pressed()) break; // 按按钮停止
    }
    beep_off();
    set_rgb(0, 0, 0);
}

void alarm_clock_test_ring(void) {
    printf("测试闹钟响铃效果...\n");
    for (int i = 0; i < 5; i++) {
        beep_on();
        set_rgb(1, 0, 0);
        usleep(500000);
        beep_off();
        set_rgb(0, 0, 0);
        usleep(500000);
    }
    printf("测试完成\n");
    printf("按回车键继续...");
    getchar();
    getchar();
}
