#include "botton.h"

// 初始化按键
void botton_init(void) {
    pinMode(KEY_PIN, INPUT);
    printf("按键初始化完成 (引脚 %d)\n", KEY_PIN);
}

// 读取按键状态
int botton_read(void) {
    return digitalRead(KEY_PIN);
}

// 检查按键是否按下（低电平）
int botton_is_pressed(void) {
    return (digitalRead(KEY_PIN) == 1);
}

// 检查按键是否释放（高电平）
int botton_is_released(void) {
    return (digitalRead(KEY_PIN) == 0);
}

