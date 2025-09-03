#ifndef TEMP_DISPLAY_H
#define TEMP_DISPLAY_H

#include "../components/DHT.h"
#include "../components/clock.h"

// 温度显示模式
typedef enum {
    TEMP_MODE_CELSIUS,    // 摄氏度模式
    TEMP_MODE_FAHRENHEIT, // 华氏度模式
    TEMP_MODE_HUMIDITY    // 湿度模式
} TempDisplayMode;

// 温度显示配置
typedef struct {
    TempDisplayMode mode;
    int update_interval;  // 更新间隔（秒）
    int max_retry;       // 最大重试次数
    int show_decimal;    // 是否显示小数点
} TempDisplayConfig;

// 函数声明
int temp_display_init(void);
int temp_display_start(TempDisplayConfig *config);
void temp_display_stop(void);
void temp_display_set_mode(TempDisplayMode mode);
int temp_display_read_sensor(DHT11_Data *data);
void temp_display_format_temperature(float temp, char *output, int show_decimal);
void temp_display_format_humidity(float humidity, char *output, int show_decimal);
void temp_display_show_error(unsigned char error_code);
void temp_display_cleanup(void);

// 信号处理
void temp_display_signal_handler(int signal);
void temp_display_setup_signal_handlers(void);

#endif // TEMP_DISPLAY_H
