#include "beep.h"

// 静态变量，只在当前文件可见
static int beep_current_state = 0;

// 初始化蜂鸣器
void beep_init(void) {
    pinMode(BEEP_PIN, OUTPUT);
    digitalWrite(BEEP_PIN, 0);
    beep_current_state = 0;
    printf("蜂鸣器初始化完成 (引脚 %d)\n", BEEP_PIN);
}

// 打开蜂鸣器
void beep_on(void) {
    digitalWrite(BEEP_PIN, 1);
    beep_current_state = 1;
    printf("蜂鸣器: 开启\n");
}

// 关闭蜂鸣器
void beep_off(void) {
    digitalWrite(BEEP_PIN, 0);
    beep_current_state = 0;
    printf("蜂鸣器: 关闭\n");
}

// 切换蜂鸣器状态
void beep_toggle(void) {
    beep_current_state = !beep_current_state;
    digitalWrite(BEEP_PIN, beep_current_state);
    printf("蜂鸣器: %s\n", beep_current_state ? "开启" : "关闭");
}

// 设置蜂鸣器状态
void beep_set_state(int state) {
    if (state) {
        beep_on();
    } else {
        beep_off();
    }
}

// 获取蜂鸣器当前状态
int beep_get_state(void) {
    return beep_current_state;
}

