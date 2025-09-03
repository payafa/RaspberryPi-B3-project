#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wiringPi.h>
#include "DHT.h"

int dht11_scan()
{
    return digitalRead(DHT_PIN);
}

void dht11_reset()
{
    pinMode(DHT_PIN, OUTPUT);
    digitalWrite(DHT_PIN, 1);
    delay(100); // 确保传感器准备好
    
    // 拉低信号至少18ms
    digitalWrite(DHT_PIN, 0);
    delay(20);
    
    // 拉高信号20-40微秒
    digitalWrite(DHT_PIN, 1);
    delayMicroseconds(30);
    
    // 设置为输入模式，等待传感器响应
    pinMode(DHT_PIN, INPUT);
}

char read_bit()
{
    int timeout = 0;
    int high_time = 0;
    
    // 等待信号变高，带超时检测
    while (dht11_scan() == 0)
    {
        delayMicroseconds(1);
        timeout++;
        if (timeout > 200) // 200微秒超时
            return 2; // 超时错误
    }
    
    // 测量高电平持续时间
    while (dht11_scan() == 1)
    {
        delayMicroseconds(1);
        high_time++;
        if (high_time > 200) // 防止死循环
            return 2;
    }
    
    // 根据高电平持续时间判断数据位
    // DHT11: 26-28微秒表示0，70微秒表示1
    if (high_time > 40)
    {
        return 1; // 数据位为1
    }
    else
    {
        return 0; // 数据位为0
    }
}

char read_byte()
{
    char i;
    unsigned char data = 0, retval;

    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        retval = read_bit();
        if (retval == 2)
        {
            return 0xcc;
        }
        data |= retval;
    }
    return data;
}

unsigned char dht11_read_data(char *buff)
{
    char i = 0;
    int timeout = 0;
    
    dht11_reset();
    
    // 等待传感器响应信号（应该拉低80微秒）
    timeout = 0;
    while (dht11_scan() == 1)
    {
        delayMicroseconds(1);
        timeout++;
        if (timeout > 500) // 增加超时时间到500微秒
            return 2; // 传感器无响应
    }
    
    // 等待传感器响应信号结束（拉高80微秒）
    timeout = 0;
    while (dht11_scan() == 0)
    {
        delayMicroseconds(1);
        timeout++;
        if (timeout > 500) // 增加超时时间
            return 2; // 传感器无响应
    }
    
    // 等待数据传输开始（等待拉低信号）
    timeout = 0;
    while (dht11_scan() == 1)
    {
        delayMicroseconds(1);
        timeout++;
        if (timeout > 500) // 增加超时时间
            return 2; // 传感器无响应
    }
    
    // 读取40位数据（5字节）
    for (i = 0; i < 5; i++)
    {
        buff[i] = read_byte();
        if (buff[i] == 0xcc) // 读取超时
            return 3;
    }
    
    // 设置引脚为输出模式并拉高
    pinMode(DHT_PIN, OUTPUT);
    digitalWrite(DHT_PIN, 1);
    
    // 校验数据
    unsigned char checksum = (buff[0] + buff[1] + buff[2] + buff[3]) & 0xFF;
    if (checksum != buff[4])
    {
        return DHT_CHECKSUM_ERROR; // 校验和错误
    }
    
    return DHT_SUCCESS; // 成功读取
}

// 新增：带重试机制的读取函数
unsigned char dht11_read_with_retry(DHT11_Data *data, int max_retry)
{
    char buffer[5];
    unsigned char result;
    int retry_count = 0;
    
    if (data == NULL || max_retry < 1) {
        return DHT_TIMEOUT_ERROR;
    }
    
    do {
        result = dht11_read_data(buffer);
        
        if (result == DHT_SUCCESS) {
            // 解析数据
            data->humidity = buffer[0] + buffer[1] * 0.1;
            data->temperature = buffer[2] + buffer[3] * 0.1;
            
            // 保存原始数据
            for (int i = 0; i < 5; i++) {
                data->raw_data[i] = buffer[i];
            }
            
            return DHT_SUCCESS;
        }
        
        retry_count++;
        if (retry_count < max_retry) {
            delay(100); // 短暂等待后重试
        }
    } while (retry_count < max_retry);
    
    return result; // 返回最后一次的错误码
}

// 新增：初始化DHT11
int dht11_init()
{
    // 初始化wiringPi
    if (wiringPiSetupGpio() == -1) {
        return -1;
    }
    
    // 设置初始状态
    pinMode(DHT_PIN, OUTPUT);
    digitalWrite(DHT_PIN, 1);
    delay(1000); // 等待传感器稳定
    
    return 0;
}
