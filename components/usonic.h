#ifndef USONIC_H
#define USONIC_H

// 超声波传感器引脚定义
#define TRIG 22
#define ECHO 23

// 函数声明
void usonic_init();
int read_dist();

#endif // USONIC_H
