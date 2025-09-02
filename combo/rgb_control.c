#include "rgb_control.h"

void rgb_button_control(void) {
    int color_index = 0;
    int colors[8][3] = {
        {0, 0, 0}, // 关闭
        {1, 0, 0}, // 红色
        {0, 1, 0}, // 绿色
        {0, 0, 1}, // 蓝色
        {1, 1, 0}, // 黄色
        {1, 0, 1}, // 紫色
        {0, 1, 1}, // 青色
        {1, 1, 1}  // 白色
    };
    
    char* color_names[] = {
        "关闭", "红色", "绿色", "蓝色", "黄色", "紫色", "青色", "白色"
    };
    
    system("clear");
    printf("\n=== 按键控制RGB灯颜色切换 ===\n");
    printf("按钮功能：按下切换下一个颜色\n");
    printf("当前颜色：%s\n", color_names[color_index]);
    printf("颜色循环：关闭→红→绿→蓝→黄→紫→青→白→关闭...\n");
    printf("按Ctrl+C退出\n\n");
    
    // 设置初始颜色
    set_rgb(colors[color_index][0], colors[color_index][1], colors[color_index][2]);
    
    while (1) {
        if (botton_is_pressed()) {
            // 防抖延时
            usleep(50000);
            
            // 切换到下一个颜色
            color_index = (color_index + 1) % 8;
            set_rgb(colors[color_index][0], colors[color_index][1], colors[color_index][2]);
            
            printf("当前颜色：%s (RGB: %d,%d,%d)\n", 
                   color_names[color_index],
                   colors[color_index][0],
                   colors[color_index][1], 
                   colors[color_index][2]);
            
            // 提示音
            beep_on();
            usleep(100000);
            beep_off();
            
            // 等待按钮释放
            while (botton_is_pressed()) {
                usleep(10000);
            }
        }
        
        usleep(50000); // 50ms延时
    }
}

void temperature_display_function(void) {
    system("clear");
    printf("\n=== 温度显示功能 ===\n");
    printf("注意：此功能需要温湿度传感器硬件支持\n");
    printf("当前为模拟演示模式\n\n");
    
    printf("模拟温度显示 (按按钮停止)...\n");
    
    // 模拟温度数据
    float temperature = 25.0;
    int display_mode = 0; // 0-温度，1-湿度
    
    while (!botton_is_pressed()) {
        if (display_mode == 0) {
            // 显示温度
            char temp_str[5];
            sprintf(temp_str, "%02d.%d", (int)temperature, (int)((temperature - (int)temperature) * 10));
            text_display(temp_str);
            
            // 根据温度设置RGB指示
            if (temperature < 20) {
                set_rgb(0, 0, 1); // 冷 - 蓝色
            } else if (temperature < 30) {
                set_rgb(0, 1, 0); // 适中 - 绿色
            } else {
                set_rgb(1, 0, 0); // 热 - 红色
            }
            
            printf("温度: %.1f°C\n", temperature);
        }
        
        // 模拟温度变化
        temperature += (rand() % 21 - 10) * 0.1; // ±1度随机变化
        if (temperature < 10) temperature = 10;
        if (temperature > 40) temperature = 40;
        
        sleep(2);
        display_mode = 1 - display_mode; // 切换显示模式
    }
    
    set_rgb(0, 0, 0);
    printf("温度显示已停止\n");
    printf("按回车键继续...");
    getchar();
    getchar();
}

void system_status_indicator(void) {
    int choice;
    
    while (1) {
        system("clear");
        printf("\n=== 系统状态指示 ===\n");
        printf("使用RGB LED显示不同的工作模式\n");
        printf("1. 正常工作模式 (绿色)\n");
        printf("2. 警告模式 (黄色闪烁)\n");
        printf("3. 错误模式 (红色快闪)\n");
        printf("4. 待机模式 (蓝色慢闪)\n");
        printf("5. 彩虹模式 (颜色循环)\n");
        printf("6. 关闭指示灯\n");
        printf("7. 返回\n");
        printf("请选择 (1-7): ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                status_normal_mode();
                break;
                
            case 2:
                status_warning_mode();
                break;
                
            case 3:
                status_error_mode();
                break;
                
            case 4:
                status_standby_mode();
                break;
                
            case 5:
                status_rainbow_mode();
                break;
                
            case 6:
                printf("指示灯已关闭\n");
                set_rgb(0, 0, 0);
                printf("按回车键继续...");
                getchar();
                getchar();
                break;
                
            case 7:
                set_rgb(0, 0, 0);
                return;
                
            default:
                printf("无效选择！\n");
                printf("按回车键继续...");
                getchar();
                getchar();
                break;
        }
        
        set_rgb(0, 0, 0); // 停止后关闭LED
        printf("模式已停止\n");
        printf("按回车键继续...");
        getchar();
        getchar();
    }
}

void status_normal_mode(void) {
    printf("正常工作模式 - 绿色常亮 (按按钮停止)\n");
    set_rgb(0, 1, 0);
    while (!botton_is_pressed()) {
        usleep(100000);
    }
}

void status_warning_mode(void) {
    printf("警告模式 - 黄色闪烁 (按按钮停止)\n");
    while (!botton_is_pressed()) {
        set_rgb(1, 1, 0);
        usleep(500000);
        set_rgb(0, 0, 0);
        usleep(500000);
    }
}

void status_error_mode(void) {
    printf("错误模式 - 红色快闪 (按按钮停止)\n");
    while (!botton_is_pressed()) {
        set_rgb(1, 0, 0);
        usleep(200000);
        set_rgb(0, 0, 0);
        usleep(200000);
    }
}

void status_standby_mode(void) {
    printf("待机模式 - 蓝色慢闪 (按按钮停止)\n");
    while (!botton_is_pressed()) {
        set_rgb(0, 0, 1);
        usleep(1000000);
        set_rgb(0, 0, 0);
        usleep(1000000);
    }
}

void status_rainbow_mode(void) {
    printf("彩虹模式 - 颜色循环 (按按钮停止)\n");
    while (!botton_is_pressed()) {
        set_rgb(1, 0, 0); usleep(300000);
        if (botton_is_pressed()) break;
        set_rgb(1, 1, 0); usleep(300000);
        if (botton_is_pressed()) break;
        set_rgb(0, 1, 0); usleep(300000);
        if (botton_is_pressed()) break;
        set_rgb(0, 1, 1); usleep(300000);
        if (botton_is_pressed()) break;
        set_rgb(0, 0, 1); usleep(300000);
        if (botton_is_pressed()) break;
        set_rgb(1, 0, 1); usleep(300000);
    }
}
