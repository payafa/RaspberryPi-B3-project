#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <time.h>
#include "usonic.h"

int read_dist() {
    time_t t1, t2;
    digitalWrite(TRIG, 1);
    usleep(10);
    digitalWrite(TRIG, 0);
    while (digitalRead(ECHO) == 0);
    t1 = micros();
    printf("t1=%ld\n", t1);
    while (digitalRead(ECHO) == 1);
    t2 = micros();
    printf("t2=%ld\n", t2);
    digitalWrite(TRIG, 0);
    sleep(1);
    return (t2 - t1) * 340 / 20000;
}

void usonic_init() {
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    digitalWrite(TRIG, 0);
    printf("超声波传感器初始化完成 (引脚 Trig:%d Echo:%d)\n", TRIG, ECHO);
    sleep(1);
}

