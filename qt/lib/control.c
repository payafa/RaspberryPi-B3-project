#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <softPwm.h>

#define LP 18
#define LN 23
#define RP 25
#define RN 12

#define SPEED 100

void setBCM(){
    if(wiringPiSetupGpio() < 0){
        perror("启动树莓派BCM失败...");
        exit(1);
    }
}

void createPwm(int pin){
    if(softPwmCreate(pin, 0, 100) == 0)printf("%d针脚初始化软件PWM成功", pin);
}

void initSoftPWM(){
    createPwm(LP);
    createPwm(LN);
    createPwm(RP);
    createPwm(RN);    
}

void setPinModeOutput(){
    pinMode(LP, OUTPUT);
    pinMode(LN, OUTPUT);
    pinMode(RP, OUTPUT);
    pinMode(RN, OUTPUT);
    printf("引脚模式OUTPUT设置完成\n");
}

void init(){
    setBCM();
    setPinModeOutput();
    initSoftPWM();
}

void stop() {
    softPwmWrite(LP, 0);
    softPwmWrite(LN, 0);
    softPwmWrite(RP, 0);
    softPwmWrite(RN, 0);
    printf("停止\n");
}

void forward(){
    softPwmWrite(LP, SPEED); 
    softPwmWrite(LN, 0);
    softPwmWrite(RP, SPEED);
    softPwmWrite(RN, 0);
    printf("前进\n");
}

void back(){
    softPwmWrite(LP, 0);
    softPwmWrite(LN, SPEED);
    softPwmWrite(RP, 0);
    softPwmWrite(RN, SPEED);
    printf("后退\n");
}

