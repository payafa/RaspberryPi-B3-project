#ifndef ALARM_CLOCK_H
#define ALARM_CLOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wiringPi.h>
#include "../components/beep.h"
#include "../components/botton.h"
#include "../components/clock.h"
#include "../components/rgb.h"

// 闹钟功能函数声明
void alarm_clock_function(void);
void alarm_clock_set_time(int *hour, int *minute);
void alarm_clock_monitor(int alarm_hour, int alarm_minute);
void alarm_clock_ring(void);
void alarm_clock_test_ring(void);

#endif // ALARM_CLOCK_H
