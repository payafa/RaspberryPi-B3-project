#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <softPwm.h>

#define WHEEL_L 23
#define WHEEL_R 24

//初始化树莓派，初始化GPIO模式为OUTPUT，初始化软件PWM，初始化R，L的GPIO低电平和初始化软件PWM值为0
void init_GPIO(){
    if (wiringPiSetupGpio() == -1) {
        printf("无法初始化WiringPi\n");
        return 1;
    }

    // 设置GPIO模式
    pinMode(WHEEL_L, OUTPUT);
    pinMode(WHEEL_R, OUTPUT);
    
    // 创建软件PWM
    softPwmCreate(WHEEL_L, 0, 100);
    softPwmCreate(WHEEL_R, 0, 100);
    
    // 初始设置
    digitalWrite(WHEEL_R, LOW);
    digitalWrite(WHEEL_L, LOW);
    softPwmWrite(WHEEL_L, 0);
    softPwmWrite(WHEEL_R, 0);
}

//清理GPIO设置
void clean_GPIO(){
    digitalWrite(WHEEL_L, LOW);
    digitalWrite(WHEEL_R, LOW);
    pinMode(WHEEL_L, INPUT);
    pinMode(WHEEL_R, INPUT);
}

//加速
void ac(char cmd[10]){
    // 移除换行符
    cmd[strcspn(cmd, "\n")] = 0;
    if (strcmp(cmd, "ac") == 0) {
        digitalWrite(WHEEL_R, LOW);
        softPwmWrite(WHEEL_L, 0);
            
        for (int dc = 0; dc <= 100; dc += 5) {
            softPwmWrite(WHEEL_L, dc);
            delay(1000);
        }
    }
}

//减速
void dc(char cmd[10]){
    if (strcmp(cmd, "dc") == 0) {
        digitalWrite(WHEEL_R, LOW);
        softPwmWrite(WHEEL_L, 100);

        for (int dc = 100; dc >= 0; dc -= 5) {
            softPwmWrite(WHEEL_L, dc);
            delay(1000);
        }
    }
}

//停止
void p(char cmd[10]){
    if (strcmp(cmd, "p") == 0) {
        digitalWrite(WHEEL_L, HIGH);
        digitalWrite(WHEEL_R, HIGH);
        pinMode(WHEEL_L, INPUT);
        pinMode(WHEEL_R, INPUT);
    }
}

//左转
void left(char cmd[10]){
    if (strcmp(cmd, "left") == 0){
        
    }

}