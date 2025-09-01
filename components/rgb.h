#ifndef RGB_H
#define RGB_H

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 引脚定义
#define R 16
#define G 20
#define B 21

// 函数声明
void rgb_init(void);
void rgb_sequence(void);
void set_rgb(int red, int green, int blue);

#endif // RGB_H
