#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wiringPi.h>
#include "../components/beep.h"
#include "../components/botton.h"
#include "../components/clock.h"
#include "../components/rgb.h"

// 秒表功能函数声明
void stopwatch_function(void);
void stopwatch_start_continue(int *running, int *start_time, int *total_seconds);
void stopwatch_pause(int *running, int *start_time, int *total_seconds);
void stopwatch_reset(int *running, int *total_seconds);
void stopwatch_button_control_mode(void);

#endif // STOPWATCH_H
