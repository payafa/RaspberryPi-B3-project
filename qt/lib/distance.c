#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <time.h>
#include <unistd.h>

#define Trig 16
#define Echo 20

int read_dist(int trig, int echo){
    unsigned int t1, t2;
    //发出触发信号
    digitalWrite(trig, 1);
    usleep(10);
    digitalWrite(trig, 0);
    //等待回应信号
    int count = 0;
    while(digitalRead(echo) == 0)
    {
        if(count >=10000000)return -1;
	    else count++;
    }
    //高电平开始计数
    t1 = micros(); 
    printf("t1 = %d\n", t1);
    while(digitalRead(echo) == 1);
    t2 = micros();
    printf("t2 = %d\n", t2);

    return (t2 - t1) * 340 * 100 / 2 / 1000000;
}

void hc_sr04_init(int trig, int echo){
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    digitalWrite(trig, 0);
    delay(1);
}

void init(){
    if(wiringPiSetupGpio() < 0){
        perror("启动树莓派wiringPi-BCM失败...");
        exit(1);
    }

    hc_sr04_init(Trig, Echo);
}

int readDist(){
    int dist = read_dist(Trig, Echo);
    printf("超声波测出距离 = %dcm\n", dist);
    return dist;
}
