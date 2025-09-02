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
void data_display(char *data);
void text_display(char *text);
void roll_display(char *data, int len);
void clock_display();

// 信号处理函数声明
void clock_signal_handler(int signal);
void clock_setup_signal_handlers(void);
void clock_cleanup(void);

#endif // CLOCK_H
