#include "control.h"

// 全局运动状态
static motion_state_t g_motion_state = {0, 0, MOTION_STOP, 0};

//初始化树莓派，初始化GPIO模式为OUTPUT，初始化软件PWM，初始化R，L的GPIO低电平和初始化软件PWM值为0
void init_wheel(){
    // 设置GPIO模式
    pinMode(WHEEL_L, OUTPUT);
    pinMode(WHEEL_R, OUTPUT);

    // 初始设置
    digitalWrite(WHEEL_R, LOW);
    digitalWrite(WHEEL_L, LOW);

    // 初始化状态
    g_motion_state.left_speed = 0;
    g_motion_state.right_speed = 0;
    g_motion_state.current_motion = MOTION_STOP;
    g_motion_state.is_moving = 0;
    
    printf("轮子控制模块初始化完成 (左轮GPIO:%d 右轮GPIO:%d)\n", WHEEL_L, WHEEL_R);
}

//清理GPIO设置
void clean_wheel(){
    control_stop(); // 先停止运动
    digitalWrite(WHEEL_L, LOW);
    digitalWrite(WHEEL_R, LOW);
    pinMode(WHEEL_L, INPUT);
    pinMode(WHEEL_R, INPUT);
    
    printf("轮子控制模块清理完成\n");
}

// Web API兼容函数：左转
void control_turn_left(int speed, int duration) {
    if (speed < 0) speed = 0;
    if (speed > MAX_SPEED) speed = MAX_SPEED;
    
    // 左转：左轮慢，右轮快 (或左轮停，右轮转)
    set_wheel_speeds(0, speed);
    g_motion_state.current_motion = MOTION_LEFT;
    g_motion_state.is_moving = 1;
    
    if (duration > 0) {
        delay(duration);
        control_stop();
    }
}

// Web API兼容函数：右转
void control_turn_right(int speed, int duration) {
    if (speed < 0) speed = 0;
    if (speed > MAX_SPEED) speed = MAX_SPEED;
    
    // 右转：右轮慢，左轮快 (或右轮停，左轮转)
    set_wheel_speeds(speed, 0);
    g_motion_state.current_motion = MOTION_RIGHT;
    g_motion_state.is_moving = 1;
    
    if (duration > 0) {
        delay(duration);
        control_stop();
    }
}

// Web API兼容函数：前进
void control_move_forward(int speed) {
    if (speed < 0) speed = 0;
    if (speed > MAX_SPEED) speed = MAX_SPEED;
    
    // 前进：两轮同速
    set_wheel_speeds(speed, speed);
    g_motion_state.current_motion = MOTION_FORWARD;
    g_motion_state.is_moving = 1;
}

// Web API兼容函数：后退
void control_move_backward(int speed) {
    if (speed < 0) speed = 0;
    if (speed > MAX_SPEED) speed = MAX_SPEED;
    
    // 后退：两轮反向同速 (这里简化为负速度，实际实现可能需要更改方向引脚)
    set_wheel_speeds(-speed, -speed);
    g_motion_state.current_motion = MOTION_BACKWARD;
    g_motion_state.is_moving = 1;
}

// Web API兼容函数：停止
void control_stop(void) {
    set_wheel_speeds(0, 0);
    g_motion_state.current_motion = MOTION_STOP;
    g_motion_state.is_moving = 0;
}

// 设置轮子速度
void set_wheel_speeds(int left_speed, int right_speed) {
    // 限制速度范围
    if (left_speed < -MAX_SPEED) left_speed = -MAX_SPEED;
    if (left_speed > MAX_SPEED) left_speed = MAX_SPEED;
    if (right_speed < -MAX_SPEED) right_speed = -MAX_SPEED;
    if (right_speed > MAX_SPEED) right_speed = MAX_SPEED;
    
    // 处理负速度 (后退)
    if (left_speed < 0) {
        // 后退逻辑 - 这里需要根据实际硬件连接调整
        digitalWrite(WHEEL_L, HIGH); // 反向
        softPwmWrite(WHEEL_L, -left_speed);
    } else {
        digitalWrite(WHEEL_L, LOW);  // 正向
        softPwmWrite(WHEEL_L, left_speed);
    }
    
    if (right_speed < 0) {
        // 后退逻辑 - 这里需要根据实际硬件连接调整
        digitalWrite(WHEEL_R, HIGH); // 反向
        softPwmWrite(WHEEL_R, -right_speed);
    } else {
        digitalWrite(WHEEL_R, LOW);  // 正向
        softPwmWrite(WHEEL_R, right_speed);
    }
    
    // 更新状态
    g_motion_state.left_speed = left_speed;
    g_motion_state.right_speed = right_speed;
}

// 获取运动状态
motion_state_t get_motion_state(void) {
    return g_motion_state;
}

// 通用运动控制
void control_motion(motion_type_t motion, int speed, int duration) {
    switch (motion) {
        case MOTION_FORWARD:
            control_move_forward(speed);
            break;
        case MOTION_BACKWARD:
            control_move_backward(speed);
            break;
        case MOTION_LEFT:
            control_turn_left(speed, duration);
            break;
        case MOTION_RIGHT:
            control_turn_right(speed, duration);
            break;
        case MOTION_STOP:
            control_stop();
            break;
        case MOTION_ACCELERATE:
            // 加速当前运动
            if (g_motion_state.is_moving) {
                int new_speed = g_motion_state.left_speed + STEP_SIZE;
                if (new_speed > MAX_SPEED) new_speed = MAX_SPEED;
                set_wheel_speeds(new_speed, new_speed);
            }
            break;
        case MOTION_DECELERATE:
            // 减速当前运动
            if (g_motion_state.is_moving) {
                int new_speed = g_motion_state.left_speed - STEP_SIZE;
                if (new_speed < 0) new_speed = 0;
                set_wheel_speeds(new_speed, new_speed);
            }
            break;
    }
}
