#ifndef BOTTON_H
#define BOTTON_H

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "beep.h"  // 包含蜂鸣器头文件

// 按键引脚定义
#define KEY_PIN 4

// 按键函数声明
void botton_init(void);
int botton_read(void);
int botton_is_pressed(void);
int botton_is_released(void);

#endif
