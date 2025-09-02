#include "clock.h"

char segdata[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, // 0~9
    0x40,                                                       // '-' (10)
    0x77,                                                       // A (11)
    0x7c,                                                       // b (12)
    0x39,                                                       // C (13)
    0x5e,                                                       // d (14)
    0x79,                                                       // E (15)
    0x71,                                                       // F (16)
    0x3d,                                                       // G (17)
    0x76,                                                       // H (18)
    0x06,                                                       // I (19)
    0x1e,                                                       // J (20)
    0x76,                                                       // K (same as H) (21)
    0x38,                                                       // L (22)
    0x15,                                                       // M (23)
    0x54,                                                       // n (24)
    0x5c,                                                       // o (25)
    0x73,                                                       // P (26)
    0x67,                                                       // q (27)
    0x50,                                                       // r (28)
    0x6d,                                                       // S (same as 5) (29)
    0x78,                                                       // t (30)
    0x3e,                                                       // U (31)
    0x3e,                                                       // V (same as U) (32)
    0x2a,                                                       // W (33)
    0x76,                                                       // X (same as H) (34)
    0x6e,                                                       // y (35)
    0x5b,                                                       // Z (same as 2) (36)
    0x00,                                                       // 空格 (37)
    0x08                                                        // 下划线 (38)
};

void tm1637_start()
{
    digitalWrite(CLK, 1);
    usleep(140);
    digitalWrite(DIO, 1);
    usleep(140);
    digitalWrite(DIO, 0);
    usleep(140);
    digitalWrite(CLK, 0);
    usleep(140);
}

void tm1637_stop()
{
    digitalWrite(CLK, 0);
    usleep(140);
    digitalWrite(DIO, 0);
    usleep(140);
    digitalWrite(CLK, 1);
    usleep(140);
    digitalWrite(DIO, 1);
    usleep(140);
}

void write_bit(char bit)
{
    digitalWrite(CLK, 0);
    usleep(140);
    if (bit)
        digitalWrite(DIO, 1);
    else
        digitalWrite(DIO, 0);
    usleep(140);
    digitalWrite(CLK, 1);
}

void write_byte(char data)
{
    char i = 0;
    for (i = 0; i < 8; i++)
    {
        write_bit((data >> i) & 0x01);
    }
    digitalWrite(CLK, 0);
    usleep(140);
    digitalWrite(DIO, 1);
    usleep(140);
    digitalWrite(CLK, 1);
    usleep(140);
    pinMode(DIO, INPUT);
    while (digitalRead(DIO))
        ;
    pinMode(DIO, OUTPUT);
}

void write_command(char cmd)
{
    tm1637_start();
    write_byte(cmd);
    tm1637_stop();
}

void write_data(char addr, char data)
{
    tm1637_start();
    write_byte(addr);
    write_byte(data);
    tm1637_stop();
}

void ascii_to_digits(char *input, int len)
{
    for (int i = 0; i < len; i++)
    {
        char c = input[i];
        if (c >= '0' && c <= '9')
        {
            input[i] = segdata[c - '0'];
        }
        else if (c == '-')
        {
            input[i] = segdata[10];
        }
        else if (c >= 'A' && c <= 'Z')
        {
            input[i] = segdata[c - 'A' + 11]; // A-Z对应索引11-36
        }
        else if (c >= 'a' && c <= 'z')
        {
            input[i] = segdata[c - 'a' + 11]; // 小写字母转为大写显示
        }
        else if (c == ' ')
        {
            input[i] = segdata[37]; // 空格
        }
        else if (c == '_')
        {
            input[i] = segdata[38]; // 下划线
        }
        else
        {
            input[i] = 0x00; // 不支持的字符显示为空
        }
    }
}

void num_display(int h_shi, int h_ge, int m_shi, int m_ge)
{
    char time_data[4] = {
        segdata[h_shi],
        segdata[h_ge],
        segdata[m_shi],
        segdata[m_ge]};
    data_display(time_data);
}

// 基础显示函数 - 直接显示4个字节的段码数据
void data_display(char *data)
{
    write_command(0x40);       // 设置数据命令
    write_command(0x44);       // 设置显示模式
    write_data(0xc0, data[0]); // 位置0
    write_data(0xc1, data[1]); // 位置1
    write_data(0xc2, data[2]); // 位置2
    write_data(0xc3, data[3]); // 位置3
    write_command(0x88);       // 开启显示，最大亮度
}

// 文本显示函数 - 将ASCII文本转换后显示
void text_display(char *text)
{
    char display_data[4] = {0x00, 0x00, 0x00, 0x00}; // 初始化为空
    int text_len = strlen(text);

    // 复制文本到显示缓冲区，最多4个字符
    for (int i = 0; i < 4 && i < text_len; i++)
    {
        display_data[i] = text[i];
    }

    // 转换为段码
    ascii_to_digits(display_data, 4);

    // 显示
    data_display(display_data);
}

// 滚动显示函数 - 重构以减少重复代码
void roll_display(char *input_data, int len)
{
    if (len <= 0)
        return;

    // 分配内存：前后各4个空格
    char *temp = malloc(len + 8);
    if (temp == NULL)
    {
        perror("内存分配失败");
        return;
    }

    // 准备滚动数据：空格 + 原数据 + 空格
    memset(temp, ' ', 4);
    memcpy(temp + 4, input_data, len);
    memset(temp + 4 + len, ' ', 4);

    // 转换为段码数据
    ascii_to_digits(temp, len + 8);

    // 滚动显示
    int i = 0;
    while (1)
    {
        data_display(temp + i);

        i = (i + 1) % (len + 4); // 使用模运算简化循环逻辑
        sleep(1);
    }

    free(temp);
}

void tm1637_init()
{
    if (wiringPiSetupGpio() < 0)
    {
        perror("init失败");
        exit(1);
    }
    pinMode(CLK, OUTPUT);
    pinMode(DIO, OUTPUT);
}

void clock_display()
{
    while (1)
    {
        time_t rawtime;
        struct tm *timeinfo;

        // 获取当前时间
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        // 直接使用重构后的时间显示函数
        int h_shi = timeinfo->tm_hour / 10;
        int h_ge = timeinfo->tm_hour % 10;
        int m_shi = timeinfo->tm_min / 10;
        int m_ge = timeinfo->tm_min % 10;

        num_display(h_shi, h_ge, m_shi, m_ge);
        sleep(30);
    }
}
