#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <softPwm.h>

#define SERVO 21

void init(){
    if(wiringPiSetupGpio() < 0){
        perror("启动树莓派wiringPi-BCM失败...");
        exit(1);
    }

    pinMode(SERVO, OUTPUT);
    softPwmCreate(SERVO, 0, 2000);
}

void spin(int value){
    softPwmWrite(SERVO, value);
}

int main(){
    init();
    int value = 0;
    while(1){
        printf("旋转角度:");
        scanf("%d", &value);
        spin(value);
    }
}