#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <softPwm.h>
#include <unistd.h>

#define LP 18
#define LN 23
#define RP 25
#define RN 12

typedef enum {
    STOP,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    SPINLEFT,
    SPINRIGHT,
    FORWARDLEFT,
    FORWARDRIGHT,
    BACKWARDLEFT,
    BACKWARDRIGHT
} ControlMode;

int current_speed = 0;
ControlMode current_mode = STOP;

void setBCM(){
    if(wiringPiSetupGpio() < 0){
        perror("启动树莓派BCM失败...");
        exit(1);
    }
}

void createPwm(int pin){
    if(softPwmCreate(pin, 0, 100) == 0)printf("%d针脚初始化软件PWM成功\n", pin);
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

// void stop() {
//     softPwmWrite(LP, 0);
//     softPwmWrite(LN, 0);
//     softPwmWrite(RP, 0);
//     softPwmWrite(RN, 0);
//     printf("停止\n");
// }
    
// void forward(){
//     softPwmWrite(LP, SPEED); 
//     softPwmWrite(LN, 0);
//     softPwmWrite(RP, SPEED);
//     softPwmWrite(RN, 0);
//     printf("前进\n");
// }

// void back(){
//     softPwmWrite(LP, 0);
//     softPwmWrite(LN, SPEED);
//     softPwmWrite(RP, 0);
//     softPwmWrite(RN, SPEED);
//     printf("后退\n");
// }

// 设置单侧电机的速度和方向
void setMotor(int pin1, int pin2, int speed, int direction) {
    if (direction == 1) {  // 正转
        softPwmWrite(pin1, speed);
        softPwmWrite(pin2, 0);
    } else if (direction == -1) {  // 反转
        softPwmWrite(pin1, 0);
        softPwmWrite(pin2, speed);
    } else {  // 停止
        softPwmWrite(pin1, 0);
        softPwmWrite(pin2, 0);
    }
}

// 设置左侧轮子
void setLeftMotor(int speed, int direction) {
    setMotor(LP, LN, speed, direction);
}

// 设置右侧轮子
void setRightMotor(int speed, int direction) {
    setMotor(RP, RN, speed, direction);
}

// 设置速度和转向比例
void setMotors(int leftSpeed, int rightSpeed, int leftDir, int rightDir) {
    setLeftMotor(leftSpeed, leftDir);
    setRightMotor(rightSpeed, rightDir);
}

// 停止
void stop() {
    setMotors(0, 0, 0, 0);
    current_mode = STOP;
    printf("停止\n");
}

// 前进
void forward(int speed) {
    setMotors(speed, speed, 1, 1);
    current_mode = FORWARD;
    current_speed = speed;
    printf("前进 - 速度: %d\n", speed);
}

// 后退
void backward(int speed) {
    setMotors(speed, speed, -1, -1);
    current_mode = BACKWARD;
    current_speed = speed;
    printf("后退 - 速度: %d\n", speed);
}

// 原地左转
void spinleft(int speed) {
    setMotors(speed, speed, -1, 1);  // 左轮后退，右轮前进
    current_mode = SPINLEFT;
    printf("原地左转 - 速度: %d\n", speed);
}

// 原地右转
void spinright(int speed) {
    setMotors(speed, speed, 1, -1);  // 左轮前进，右轮后退
    current_mode = SPINRIGHT;
    printf("原地右转 - 速度: %d\n", speed);
}

// 前进左转（弧线）
void forwardleft(int speed, int turnRatio) {
    int rightSpeed = speed;
    int leftSpeed = speed * (100 - turnRatio) / 100;
    setMotors(leftSpeed, rightSpeed, 1, 1);
    current_mode = FORWARDLEFT;
    printf("前进左转 - 速度: %d, 转向比: %d%%\n", speed, turnRatio);
}

// 前进右转（弧线）
void forwardright(int speed, int turnRatio) {
    int leftSpeed = speed;
    int rightSpeed = speed * (100 - turnRatio) / 100;
    setMotors(leftSpeed, rightSpeed, 1, 1);
    current_mode = FORWARDRIGHT;
    printf("前进右转 - 速度: %d, 转向比: %d%%\n", speed, turnRatio);
}

// 后退左转
void backwardleft(int speed, int turnRatio) {
    int rightSpeed = speed;
    int leftSpeed = speed * (100 - turnRatio) / 100;
    setMotors(leftSpeed, rightSpeed, -1, -1);
    current_mode = BACKWARDLEFT;
    printf("后退左转 - 速度: %d, 转向比: %d%%\n", speed, turnRatio);
}

// 后退右转
void backwardright(int speed, int turnRatio) {
    int leftSpeed = speed;
    int rightSpeed = speed * (100 - turnRatio) / 100;
    setMotors(leftSpeed, rightSpeed, -1, -1);
    current_mode = BACKWARDRIGHT;
    printf("后退右转 - 速度: %d, 转向比: %d%%\n", speed, turnRatio);
}