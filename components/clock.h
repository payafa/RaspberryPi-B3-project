#ifndef CLOCK_H
#define CLOCK_H

#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

// 引脚定义
#define DIO 22
#define CLK 27

// 段码数据
extern char segdata[];

// 函数声明
void tm1637_init(void);
void tm1637_start(void);
void tm1637_stop(void);
void write_bit(char bit);
void write_byte(char data);
void write_command(char cmd);
void write_data(char addr, char data);
void ascii_to_digits(char *input, int len);
void time_display(int h_shi, int h_ge, int m_shi, int m_ge);
void data_display(char *data);
void catch_sigint(int signo);
void roll_display(char *data, int len);
void show_clock();
#endif // CLOCK_H
