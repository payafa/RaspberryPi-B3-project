#ifndef SERVO_H
#define SERVO_H

#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// 舵机引脚定义
#define SERVO_PIN 18  // GPIO 18引脚

// 舵机PWM参数
#define SERVO_PWM_RANGE 200    // PWM范围 (对应20ms周期)
#define SERVO_MIN_PULSE 5      // 最小脉宽 (对应0度，约1ms)
#define SERVO_MAX_PULSE 25     // 最大脉宽 (对应180度，约2.5ms)
#define SERVO_CENTER_PULSE 15  // 中心脉宽 (对应90度，约1.5ms)

// 角度限制
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180

// 函数声明
void servo_init(void);
void servo_set_angle(int angle);
void servo_sweep(void);
void servo_demo(void);
int angle_to_pwm(int angle);

// 信号处理函数声明
void servo_signal_handler(int signal);
void servo_setup_signal_handlers(void);
void servo_cleanup(void);
int servo_is_running(void);

#endif // SERVO_H
