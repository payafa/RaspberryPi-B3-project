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
#include "components/DHT.h"
#include "components/usonic.h"
#include "components/servo.h"
#include "components/control.h"  // 新增运动控制
#include "combo/alarm_clock.h"
#include "combo/stopwatch.h"
#include "combo/rgb_control.h"
#include "combo/temp_display.h"

// 函数声明
void show_main_menu(void);
void show_single_component_menu(void);
void show_combo_component_menu(void);
void test_beep(void);
void test_button(void);
void test_clock(void);
void test_rgb(void);
void test_dht_sensor(void);
void test_usonic_sensor(void);
void test_servo(void);
void test_motion_control(void);  // 新增运动控制测试
void test_temp_display(void);
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
    usonic_init();
    control_init();  // 新增运动控制初始化
    
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
        printf("║  5. DHT11温湿度传感器测试            ║\n");
        printf("║  6. 超声波距离传感器测试             ║\n");
        printf("║  7. 舵机控制测试                     ║\n");
        printf("║  8. 运动控制测试                     ║\n");
        printf("║  9. 返回主菜单                       ║\n");
        printf("╚══════════════════════════════════════╝\n");
        printf("\n");
        
        printf("请选择测试项目 (1-9): ");
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
                test_dht_sensor();
                break;
            case 6:
                test_usonic_sensor();
                break;
            case 7:
                test_servo();
                break;
            case 8:
                test_motion_control();
                break;
            case 9:
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
                test_temp_display();
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
            printf("蜂鸣器切换测试 (按Ctrl+C返回菜单)...\n");
            beep_setup_signal_handlers();
            while (beep_is_running()) {
                beep_toggle();
                usleep(500000); // 0.5秒
            }
            beep_cleanup();
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
    printf("请按下按键进行测试 (按Ctrl+C返回菜单)\n");
    printf("按键状态监测中...\n\n");
    
    // 设置信号处理
    beep_setup_signal_handlers();
    
    while (beep_is_running()) {
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
    
    // 退出时清理
    beep_cleanup();
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
            printf("显示当前时间 (按Ctrl+C返回菜单)...\n");
            clock_setup_signal_handlers();
            clock_display(); // 函数会在接收到信号时自动返回
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
            printf("滚动显示文本: %s (按Ctrl+C返回菜单)...\n", text);
            clock_setup_signal_handlers();
            roll_display(text, strlen(text)); // 函数会在接收到信号时自动返回
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
            printf("RGB序列演示 (按Ctrl+C返回菜单)...\n");
            rgb_setup_signal_handlers();
            rgb_sequence(); // 函数会在接收到信号时自动返回
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

// DHT11温湿度传感器测试
void test_dht_sensor(void) {
    int choice;
    DHT11_Data sensor_data;
    unsigned char result;
    
    clear_screen();
    printf("\n=== DHT11温湿度传感器测试 ===\n");
    printf("1. 读取温湿度数据\n");
    printf("2. 连续监测模式\n");
    printf("3. 返回\n");
    printf("请选择 (1-3): ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            printf("\n正在读取传感器数据...\n");
            result = dht11_read_with_retry(&sensor_data, 3);
            
            if (result == DHT_SUCCESS) {
                printf("读取成功！\n");
                printf("温度: %.1f°C\n", sensor_data.temperature);
                printf("湿度: %.1f%%\n", sensor_data.humidity);
            } else {
                printf("读取失败！错误代码: %d\n", result);
                switch (result) {
                    case DHT_CHECKSUM_ERROR:
                        printf("错误原因: 数据校验失败\n");
                        break;
                    case DHT_NO_RESPONSE:
                        printf("错误原因: 传感器无响应\n");
                        break;
                    case DHT_TIMEOUT_ERROR:
                        printf("错误原因: 读取超时\n");
                        break;
                }
            }
            wait_for_input();
            break;
            
        case 2:
            printf("\n进入连续监测模式 (按Ctrl+C退出)...\n");
            printf("每2秒读取一次数据\n\n");
            
            int count = 0;
            int success = 0;
            
            while (count < 10) { // 读取10次后自动退出
                result = dht11_read_with_retry(&sensor_data, 3);
                count++;
                
                if (result == DHT_SUCCESS) {
                    success++;
                    printf("[%02d] 温度: %.1f°C, 湿度: %.1f%% (成功率: %.1f%%)\n", 
                           count, sensor_data.temperature, sensor_data.humidity,
                           (float)success/count*100);
                } else {
                    printf("[%02d] 读取失败 (错误代码: %d, 成功率: %.1f%%)\n", 
                           count, result, (float)success/count*100);
                }
                
                if (count < 10) sleep(2);
            }
            
            printf("\n监测完成！总成功率: %.1f%%\n", (float)success/count*100);
            wait_for_input();
            break;
            
        case 3:
            return;
            
        default:
            printf("无效选择！\n");
            wait_for_input();
            break;
    }
}

void test_usonic_sensor(void) {
    int choice;
    int distance;
    
    while (1) {
        clear_screen();
        printf("\n=== 超声波距离传感器测试 ===\n");
        printf("1. 测量距离 (单次)\n");
        printf("2. 连续测量距离\n");
        printf("3. 返回\n");
        printf("请选择 (1-3): ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("\n正在测量距离...\n");
                distance = read_dist();
                if (distance > 0) {
                    printf("测量距离: %d cm\n", distance);
                } else {
                    printf("测量失败，请检查传感器连接\n");
                }
                wait_for_input();
                break;
                
            case 2:
                printf("\n连续测量模式 (按Ctrl+C停止)\n");
                printf("每2秒测量一次距离...\n");
                while (1) {
                    distance = read_dist();
                    if (distance > 0) {
                        printf("距离: %d cm\n", distance);
                    } else {
                        printf("测量失败\n");
                    }
                    sleep(2);
                }
                break;

            case 3:
                return;
                
            default:
                printf("无效选择，请重新输入！\n");
                wait_for_input();
                break;
        }
    }
}

// 舵机控制测试
void test_servo(void) {
    int choice;
    int angle;
    
    while (1) {
        clear_screen();
        printf("\n=== 舵机控制测试 ===\n");
        printf("1. 设置舵机角度\n");
        printf("2. 舵机扫描演示\n");
        printf("3. 舵机完整演示\n");
        printf("4. 返回\n");
        printf("请选择 (1-4): ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("\n请输入目标角度 (0-180): ");
                scanf("%d", &angle);
                if (angle >= 0 && angle <= 180) {
                    printf("正在初始化舵机...\n");
                    servo_init();
                    printf("设置舵机角度为 %d 度...\n", angle);
                    servo_set_angle(angle);
                    printf("舵机角度设置完成\n");
                } else {
                    printf("角度范围错误！请输入0-180之间的角度\n");
                }
                wait_for_input();
                break;
                
            case 2:
                printf("\n开始舵机扫描演示...\n");
                printf("舵机将在0-180度之间来回扫描\n");
                printf("按Ctrl+C停止演示\n");
                servo_init();
                servo_sweep();
                break;

            case 3:
                printf("\n开始舵机完整演示...\n");
                printf("包含角度测试和扫描演示\n");
                printf("按Ctrl+C中断演示\n");
                servo_demo();
                break;

            case 4:
                return;
                
            default:
                printf("无效选择，请重新输入！\n");
                wait_for_input();
                break;
        }
    }
}

// 温度显示功能（数码管显示）
void test_temp_display(void) {
    int choice;
    TempDisplayConfig config;
    
    clear_screen();
    printf("\n=== 温度显示功能 ===\n");
    printf("1. 显示摄氏温度\n");
    printf("2. 显示华氏温度\n");
    printf("3. 显示湿度\n");
    printf("4. 自定义显示模式\n");
    printf("5. 返回\n");
    printf("请选择 (1-5): ");
    scanf("%d", &choice);
    
    // 设置默认配置
    config.update_interval = 2;
    config.max_retry = 3;
    config.show_decimal = 1;
    
    switch (choice) {
        case 1:
            config.mode = TEMP_MODE_CELSIUS;
            printf("\n启动摄氏温度显示模式...\n");
            printf("温度将显示在数码管上，按Ctrl+C退出\n");
            
            if (temp_display_init() == 0) {
                temp_display_start(&config);
                temp_display_cleanup();
            }
            break;
            
        case 2:
            config.mode = TEMP_MODE_FAHRENHEIT;
            printf("\n启动华氏温度显示模式...\n");
            printf("温度将显示在数码管上，按Ctrl+C退出\n");
            
            if (temp_display_init() == 0) {
                temp_display_start(&config);
                temp_display_cleanup();
            }
            break;
            
        case 3:
            config.mode = TEMP_MODE_HUMIDITY;
            printf("\n启动湿度显示模式...\n");
            printf("湿度将显示在数码管上，按Ctrl+C退出\n");
            
            if (temp_display_init() == 0) {
                temp_display_start(&config);
                temp_display_cleanup();
            }
            break;
            
        case 4:
            printf("\n=== 自定义显示模式 ===\n");
            printf("选择显示内容:\n");
            printf("1. 摄氏温度  2. 华氏温度  3. 湿度\n");
            printf("请选择 (1-3): ");
            
            int mode_choice;
            scanf("%d", &mode_choice);
            
            switch (mode_choice) {
                case 1: config.mode = TEMP_MODE_CELSIUS; break;
                case 2: config.mode = TEMP_MODE_FAHRENHEIT; break;
                case 3: config.mode = TEMP_MODE_HUMIDITY; break;
                default: 
                    printf("无效选择，使用默认摄氏温度模式\n");
                    config.mode = TEMP_MODE_CELSIUS;
                    break;
            }
            
            printf("设置更新间隔 (秒, 1-10): ");
            scanf("%d", &config.update_interval);
            if (config.update_interval < 1 || config.update_interval > 10) {
                config.update_interval = 2;
                printf("无效间隔，使用默认2秒\n");
            }
            
            printf("是否显示小数点? (1=是, 0=否): ");
            scanf("%d", &config.show_decimal);
            
            printf("\n启动自定义显示模式...\n");
            printf("显示内容将出现在数码管上，按Ctrl+C退出\n");
            
            if (temp_display_init() == 0) {
                temp_display_start(&config);
                temp_display_cleanup();
            }
            break;
            
        case 5:
            return;
            
        default:
            printf("无效选择！\n");
            wait_for_input();
            break;
    }
}

// 运动控制测试函数
void test_motion_control(void) {
    int choice;
    int speed = 50; // 默认速度50%
    
    printf("\n");
    printf("╔══════════════════════════════════════╗\n");
    printf("║          运动控制测试                ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║  测试轮子运动控制功能                ║\n");
    printf("║  注意：确保轮子已正确连接            ║\n");
    printf("╚══════════════════════════════════════╝\n");
    printf("\n");
    
    while (1) {
        printf("\n");
        printf("╔══════════════════════════════════════╗\n");
        printf("║          运动控制菜单                ║\n");
        printf("╠══════════════════════════════════════╣\n");
        printf("║  1. 前进                             ║\n");
        printf("║  2. 后退                             ║\n");
        printf("║  3. 左转                             ║\n");
        printf("║  4. 右转                             ║\n");
        printf("║  5. 停止                             ║\n");
        printf("║  6. 设置速度 (当前: %d%%)             ║\n", speed);
        printf("║  7. 显示运动状态                     ║\n");
        printf("║  8. 返回上级菜单                     ║\n");
        printf("╚══════════════════════════════════════╝\n");
        printf("\n");
        
        printf("请选择操作 (1-8): ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("执行前进运动 (速度: %d%%)...\n", speed);
                control_move_forward(speed);
                printf("前进命令已发送\n");
                sleep(1);
                break;
                
            case 2:
                printf("执行后退运动 (速度: %d%%)...\n", speed);
                control_move_backward(speed);
                printf("后退命令已发送\n");
                sleep(1);
                break;
                
            case 3:
                printf("执行左转运动 (速度: %d%%)...\n", speed);
                control_turn_left(speed, 1000); // 左转1秒
                printf("左转命令已发送\n");
                sleep(1);
                break;
                
            case 4:
                printf("执行右转运动 (速度: %d%%)...\n", speed);
                control_turn_right(speed, 1000); // 右转1秒
                printf("右转命令已发送\n");
                sleep(1);
                break;
                
            case 5:
                printf("停止所有运动...\n");
                control_stop();
                printf("停止命令已发送\n");
                sleep(1);
                break;
                
            case 6:
                printf("当前速度: %d%%\n", speed);
                printf("请输入新速度 (0-100): ");
                scanf("%d", &speed);
                if (speed < 0 || speed > 100) {
                    speed = 50;
                    printf("无效速度，重置为50%%\n");
                } else {
                    printf("速度已设置为: %d%%\n", speed);
                }
                sleep(1);
                break;
                
            case 7: {
                motion_state_t state = get_motion_state();
                printf("\n--- 运动状态信息 ---\n");
                printf("当前方向: ");
                switch (state.current_motion) {
                    case MOTION_FORWARD:  printf("前进\n"); break;
                    case MOTION_BACKWARD: printf("后退\n"); break;
                    case MOTION_LEFT:     printf("左转\n"); break;
                    case MOTION_RIGHT:    printf("右转\n"); break;
                    case MOTION_STOP:     printf("停止\n"); break;
                    case MOTION_ACCELERATE: printf("加速\n"); break;
                    case MOTION_DECELERATE: printf("减速\n"); break;
                    default:              printf("未知\n"); break;
                }
                printf("左轮速度: %d%%\n", state.left_speed);
                printf("右轮速度: %d%%\n", state.right_speed);
                printf("运动状态: %s\n", state.is_moving ? "运动中" : "静止");
                printf("-------------------\n");
                wait_for_input();
                break;
            }
                
            case 8:
                printf("停止运动并返回...\n");
                control_stop();
                return;
                
            default:
                printf("无效选择！\n");
                wait_for_input();
                break;
        }
    }
}
