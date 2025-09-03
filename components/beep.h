#ifndef BEEP_H
#define BEEP_H

#include <wiringPi.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

// 蜂鸣器引脚定义
#define BEEP_PIN 18

// 蜂鸣器函数声明
void beep_init(void);
void beep_on(void);
void beep_off(void);
void beep_toggle(void);
void beep_set_state(int state);
int beep_get_state(void);

// 信号处理函数声明
void beep_signal_handler(int signal);
void beep_setup_signal_handlers(void);
int beep_is_running(void);
void beep_cleanup(void);

#endif
