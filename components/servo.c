#include "servo.h"

// 静态变量
static volatile int servo_signal_received = 0;
static volatile int servo_running = 1;

// 信号处理函数
void servo_signal_handler(int signal)
{
    servo_signal_received = signal;
    printf("\n舵机组件接收到信号 %d\n", signal);
    
    switch (signal)
    {
        case SIGINT:  // Ctrl+C - 设置停止标志
            printf("舵机组件: 接收到退出信号，准备返回主菜单...\n");
            servo_running = 0;
            break;
        default:
            printf("舵机组件: 未处理的信号 %d\n", signal);
            break;
    }
}

// 设置信号处理器
void servo_setup_signal_handlers(void)
{
    signal(SIGINT, servo_signal_handler);
    printf("舵机信号处理器设置完成 (仅SIGINT)\n");
}

// 清理函数
void servo_cleanup(void)
{
    printf("舵机组件清理中...\n");
    servo_set_angle(90); // 回到中位
    delay(500);
    servo_running = 1; // 重置运行状态以便下次使用
    printf("舵机组件清理完成\n");
}

// 获取运行状态
int servo_is_running(void)
{
    return servo_running;
}

// 舵机初始化
void servo_init(void)
{
    if (wiringPiSetupGpio() < 0)
    {
        printf("舵机: wiringPi初始化失败\n");
        exit(1);
    }
    
    // 设置引脚为输出模式
    pinMode(SERVO_PIN, OUTPUT);
    digitalWrite(SERVO_PIN, LOW);
    
    // 创建软件PWM
    if (softPwmCreate(SERVO_PIN, 0, SERVO_PWM_RANGE) != 0)
    {
        printf("舵机: 软件PWM创建失败\n");
        exit(1);
    }
    
    printf("舵机初始化完成，使用GPIO %d引脚\n", SERVO_PIN);
    
    // 初始化到中位(90度)
    servo_set_angle(90);
    delay(1000);
}

// 角度转换为PWM值
int angle_to_pwm(int angle)
{
    // 限制角度范围
    if (angle < SERVO_MIN_ANGLE) angle = SERVO_MIN_ANGLE;
    if (angle > SERVO_MAX_ANGLE) angle = SERVO_MAX_ANGLE;
    
    // 将角度(0-180)映射到PWM值(SERVO_MIN_PULSE - SERVO_MAX_PULSE)
    int pwm_value = SERVO_MIN_PULSE + 
                   (angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE)) / SERVO_MAX_ANGLE;
    
    return pwm_value;
}

// 设置舵机角度
void servo_set_angle(int angle)
{
    int pwm_value = angle_to_pwm(angle);
    softPwmWrite(SERVO_PIN, pwm_value);
    printf("舵机设置角度: %d° (PWM值: %d)\n", angle, pwm_value);
}

// 舵机扫描演示 (0度到180度来回扫描)
void servo_sweep(void)
{
    printf("开始舵机扫描演示 (按Ctrl+C停止)\n");
    servo_setup_signal_handlers();
    
    int angle = 0;
    int direction = 1; // 1为正向，-1为反向
    int step = 5; // 每次移动5度
    
    while (servo_running)
    {
        servo_set_angle(angle);
        delay(100); // 延时100ms
        
        angle += direction * step;
        
        // 到达边界时改变方向
        if (angle >= SERVO_MAX_ANGLE)
        {
            angle = SERVO_MAX_ANGLE;
            direction = -1;
            printf("到达180度，开始反向扫描\n");
        }
        else if (angle <= SERVO_MIN_ANGLE)
        {
            angle = SERVO_MIN_ANGLE;
            direction = 1;
            printf("到达0度，开始正向扫描\n");
        }
    }
    
    servo_cleanup();
}

// 舵机演示程序
void servo_demo(void)
{
    printf("=== 舵机控制演示 ===\n");
    printf("舵机控制范围: 0° - 180°\n");
    printf("使用GPIO %d引脚\n", SERVO_PIN);
    printf("PWM范围: %d (20ms周期)\n", SERVO_PWM_RANGE);
    printf("脉宽范围: %d - %d (对应0° - 180°)\n", SERVO_MIN_PULSE, SERVO_MAX_PULSE);
    
    servo_init();
    servo_setup_signal_handlers();
    
    printf("\n开始舵机测试序列...\n");
    
    // 测试几个关键位置
    int test_angles[] = {0, 45, 90, 135, 180, 90};
    int num_angles = sizeof(test_angles) / sizeof(test_angles[0]);
    
    for (int i = 0; i < num_angles && servo_running; i++)
    {
        printf("移动到 %d°...\n", test_angles[i]);
        servo_set_angle(test_angles[i]);
        delay(1500); // 等待1.5秒
    }
    
    if (servo_running)
    {
        printf("\n现在开始连续扫描演示...\n");
        servo_sweep();
    }
    
    servo_cleanup();
    printf("舵机演示结束\n");
}
