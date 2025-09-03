#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <wiringPi.h>
#include "temp_display.h"

// 全局变量
static int running = 0;
static TempDisplayConfig current_config;

// 初始化温度显示系统
int temp_display_init(void)
{
    // 初始化DHT11
    if (dht11_init() != 0) {
        printf("DHT11初始化失败!\n");
        return -1;
    }
    
    // 初始化数码管
    tm1637_init();
    
    // 设置默认配置
    current_config.mode = TEMP_MODE_CELSIUS;
    current_config.update_interval = 2;
    current_config.max_retry = 3;
    current_config.show_decimal = 1;
    
    printf("温度显示系统初始化成功!\n");
    return 0;
}

// 读取传感器数据
int temp_display_read_sensor(DHT11_Data *data)
{
    return dht11_read_with_retry(data, current_config.max_retry);
}

// 格式化温度显示
void temp_display_format_temperature(float temp, char *output, int show_decimal)
{
    if (show_decimal) {
        snprintf(output, 5, "%.1f", temp);
        // 移除小数点，因为数码管会自动显示
        char temp_str[5];
        sprintf(temp_str, "%02d%02d", (int)temp, (int)((temp - (int)temp) * 10));
        strcpy(output, temp_str);
    } else {
        snprintf(output, 5, "%02d  ", (int)temp);
    }
}

// 格式化湿度显示
void temp_display_format_humidity(float humidity, char *output, int show_decimal)
{
    if (show_decimal) {
        snprintf(output, 5, "%.1f", humidity);
        // 移除小数点，因为数码管会自动显示
        char temp_str[5];
        sprintf(temp_str, "%02d%02d", (int)humidity, (int)((humidity - (int)humidity) * 10));
        strcpy(output, temp_str);
    } else {
        snprintf(output, 5, "%02d  ", (int)humidity);
    }
}

// 显示错误信息
void temp_display_show_error(unsigned char error_code)
{
    char error_msg[5];
    
    switch (error_code) {
        case DHT_CHECKSUM_ERROR:
            strcpy(error_msg, "Err1");
            break;
        case DHT_NO_RESPONSE:
            strcpy(error_msg, "Err2");
            break;
        case DHT_TIMEOUT_ERROR:
            strcpy(error_msg, "Err3");
            break;
        default:
            strcpy(error_msg, "Err ");
            break;
    }
    
    data_display(error_msg);
}

// 设置显示模式
void temp_display_set_mode(TempDisplayMode mode)
{
    current_config.mode = mode;
    
    const char* mode_names[] = {"摄氏度", "华氏度", "湿度"};
    printf("切换到%s模式\n", mode_names[mode]);
}

// 信号处理函数
void temp_display_signal_handler(int signal)
{
    if (signal == SIGINT || signal == SIGTERM) {
        printf("\n收到退出信号，正在清理资源...\n");
        running = 0;
    }
}

// 设置信号处理
void temp_display_setup_signal_handlers(void)
{
    signal(SIGINT, temp_display_signal_handler);
    signal(SIGTERM, temp_display_signal_handler);
}

// 主显示循环
int temp_display_start(TempDisplayConfig *config)
{
    if (config != NULL) {
        current_config = *config;
    }
    
    temp_display_setup_signal_handlers();
    
    DHT11_Data sensor_data;
    char display_buffer[5];
    unsigned char result;
    int success_count = 0;
    int total_count = 0;
    
    running = 1;
    printf("温度显示系统启动 (按Ctrl+C退出)\n");
    printf("当前模式: %s\n", 
           current_config.mode == TEMP_MODE_CELSIUS ? "摄氏度" :
           current_config.mode == TEMP_MODE_FAHRENHEIT ? "华氏度" : "湿度");
    
    while (running) {
        total_count++;
        result = temp_display_read_sensor(&sensor_data);
        
        if (result == DHT_SUCCESS) {
            success_count++;
            
            switch (current_config.mode) {
                case TEMP_MODE_CELSIUS:
                    temp_display_format_temperature(sensor_data.temperature, 
                                                   display_buffer, 
                                                   current_config.show_decimal);
                    printf("温度: %.1f°C [成功率: %.1f%%]\n", 
                           sensor_data.temperature,
                           (float)success_count/total_count*100);
                    break;
                    
                case TEMP_MODE_FAHRENHEIT:
                    {
                        float fahrenheit = sensor_data.temperature * 9.0 / 5.0 + 32.0;
                        temp_display_format_temperature(fahrenheit, 
                                                       display_buffer, 
                                                       current_config.show_decimal);
                        printf("温度: %.1f°F [成功率: %.1f%%]\n", 
                               fahrenheit,
                               (float)success_count/total_count*100);
                    }
                    break;
                    
                case TEMP_MODE_HUMIDITY:
                    temp_display_format_humidity(sensor_data.humidity, 
                                                display_buffer, 
                                                current_config.show_decimal);
                    printf("湿度: %.1f%% [成功率: %.1f%%]\n", 
                           sensor_data.humidity,
                           (float)success_count/total_count*100);
                    break;
            }
            
            data_display(display_buffer);
        } else {
            // 显示错误
            temp_display_show_error(result);
            
            const char* error_names[] = {"校验错误", "无响应", "超时"};
            printf("传感器错误: %s [成功率: %.1f%%]\n", 
                   error_names[result], 
                   (float)success_count/total_count*100);
        }
        
        sleep(current_config.update_interval);
    }
    
    return 0;
}

// 停止显示
void temp_display_stop(void)
{
    running = 0;
}

// 清理资源
void temp_display_cleanup(void)
{
    // 清空数码管显示
    data_display("    ");
    printf("温度显示系统已关闭\n");
}

// 主函数（用于测试）
int main(int argc, char *argv[])
{
    TempDisplayConfig config;
    
    // 解析命令行参数
    config.mode = TEMP_MODE_CELSIUS;
    config.update_interval = 2;
    config.max_retry = 3;
    config.show_decimal = 1;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--humidity") == 0) {
            config.mode = TEMP_MODE_HUMIDITY;
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fahrenheit") == 0) {
            config.mode = TEMP_MODE_FAHRENHEIT;
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            config.update_interval = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("温度显示程序\n");
            printf("用法: %s [选项]\n", argv[0]);
            printf("选项:\n");
            printf("  -h, --humidity    显示湿度\n");
            printf("  -f, --fahrenheit  显示华氏温度\n");
            printf("  -i <秒数>         设置更新间隔\n");
            printf("  --help           显示此帮助信息\n");
            return 0;
        }
    }
    
    // 初始化系统
    if (temp_display_init() != 0) {
        return -1;
    }
    
    // 启动显示
    temp_display_start(&config);
    
    // 清理资源
    temp_display_cleanup();
    
    return 0;
}
