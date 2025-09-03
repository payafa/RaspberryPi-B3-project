#ifndef DHT_H
#define DHT_H

// DHT11传感器引脚定义
#define DHT_PIN 13

// 返回值定义
#define DHT_SUCCESS         1   // 成功读取数据
#define DHT_CHECKSUM_ERROR  0   // 校验和错误
#define DHT_NO_RESPONSE     2   // 传感器无响应
#define DHT_TIMEOUT_ERROR   3   // 数据读取超时

// DHT11数据结构
typedef struct {
    float humidity;     // 湿度
    float temperature;  // 温度
    unsigned char raw_data[5]; // 原始数据
} DHT11_Data;

// 函数声明
int dht11_scan(void);
void dht11_reset(void);
char read_bit(void);
char read_byte(void);
unsigned char dht11_read_data(char *buff);
unsigned char dht11_read_with_retry(DHT11_Data *data, int max_retry);
int dht11_init(void);

#endif // DHT_H
