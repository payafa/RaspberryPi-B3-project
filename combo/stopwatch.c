#include "stopwatch.h"

void stopwatch_function(void) {
    int choice;
    int running = 0;
    int total_seconds = 0;
    int start_time = 0;
    
    while (1) {
        system("clear");
        printf("\n=== 秒表功能 ===\n");
        printf("当前计时: %02d:%02d\n", total_seconds / 60, total_seconds % 60);
        printf("状态: %s\n", running ? "运行中" : "已暂停");
        printf("1. 开始/继续计时\n");
        printf("2. 暂停计时\n");
        printf("3. 重置计时\n");
        printf("4. 按钮控制模式\n");
        printf("5. 返回\n");
        printf("请选择 (1-5): ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                stopwatch_start_continue(&running, &start_time, &total_seconds);
                break;
                
            case 2:
                stopwatch_pause(&running, &start_time, &total_seconds);
                break;
                
            case 3:
                stopwatch_reset(&running, &total_seconds);
                break;
                
            case 4:
                stopwatch_button_control_mode();
                break;
                
            case 5:
                running = 0;
                set_rgb(0, 0, 0);
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

void stopwatch_start_continue(int *running, int *start_time, int *total_seconds) {
    if (!(*running)) {
        *running = 1;
        *start_time = time(NULL) - *total_seconds;
        printf("计时开始！\n");
        set_rgb(0, 1, 0); // 绿色表示运行
    } else {
        printf("计时已在运行中！\n");
    }
    printf("按回车键继续...");
    getchar();
    getchar();
}

void stopwatch_pause(int *running, int *start_time, int *total_seconds) {
    if (*running) {
        *running = 0;
        *total_seconds = time(NULL) - *start_time;
        printf("计时暂停！\n");
        set_rgb(1, 1, 0); // 黄色表示暂停
    } else {
        printf("计时已暂停！\n");
    }
    printf("按回车键继续...");
    getchar();
    getchar();
}

void stopwatch_reset(int *running, int *total_seconds) {
    *running = 0;
    *total_seconds = 0;
    printf("计时重置！\n");
    text_display("0000");
    set_rgb(0, 0, 0); // 关闭指示灯
    printf("按回车键继续...");
    getchar();
    getchar();
}

void stopwatch_button_control_mode(void) {
    int running = 0;
    int total_seconds = 0;
    int start_time = 0;
    
    printf("按钮控制模式：短按开始/暂停，长按重置\n");
    printf("按Ctrl+C退出此模式\n");
    
    while (1) {
        if (running) {
            total_seconds = time(NULL) - start_time;
            // 确保时间值不为负数
            if (total_seconds < 0) total_seconds = 0;
            
            char time_str[10]; // 增加缓冲区大小以防止溢出
            int minutes = total_seconds / 60;
            int seconds = total_seconds % 60;
            
            // 限制显示范围，防止溢出
            if (minutes > 99) minutes = 99;
            if (seconds < 0) seconds = 0;
            if (seconds > 59) seconds = 59;
            
            sprintf(time_str, "%02d%02d", minutes, seconds);
            text_display(time_str);
        }
        
        if (botton_is_pressed()) {
            // 简单的短按检测
            usleep(50000); // 防抖
            if (!running) {
                running = 1;
                start_time = time(NULL) - total_seconds;
                set_rgb(0, 1, 0);
                beep_on();
                usleep(100000);
                beep_off();
            } else {
                running = 0;
                total_seconds = time(NULL) - start_time;
                set_rgb(1, 1, 0);
                beep_on();
                usleep(200000);
                beep_off();
            }
            
            // 等待按钮释放
            while (botton_is_pressed()) {
                usleep(10000);
            }
        }
        
        usleep(100000); // 0.1秒延时
    }
}
