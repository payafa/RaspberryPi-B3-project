#ifndef RGB_CONTROL_H
#define RGB_CONTROL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include "../components/beep.h"
#include "../components/botton.h"
#include "../components/clock.h"
#include "../components/rgb.h"

// RGB控制功能函数声明
void rgb_button_control(void);
void temperature_display_function(void);
void system_status_indicator(void);

// 系统状态指示模式
void status_normal_mode(void);
void status_warning_mode(void);
void status_error_mode(void);
void status_standby_mode(void);
void status_rainbow_mode(void);

#endif // RGB_CONTROL_H
