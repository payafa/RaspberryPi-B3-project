#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "temp_display.h"

// 独立的温度显示主程序
int main(int argc, char *argv[])
{
    TempDisplayConfig config;
    
    // 解析命令行参数
    config.mode = TEMP_MODE_CELSIUS;
    config.update_interval = 2;
    config.max_retry = 3;
    config.show_decimal = 1;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--humidity") == 0) {
            config.mode = TEMP_MODE_HUMIDITY;
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fahrenheit") == 0) {
            config.mode = TEMP_MODE_FAHRENHEIT;
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            config.update_interval = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("温度显示程序\n");
            printf("用法: %s [选项]\n", argv[0]);
            printf("选项:\n");
            printf("  -h, --humidity    显示湿度\n");
            printf("  -f, --fahrenheit  显示华氏温度\n");
            printf("  -i <秒数>         设置更新间隔\n");
            printf("  --help           显示此帮助信息\n");
            return 0;
        }
    }
    
    // 初始化系统
    if (temp_display_init() != 0) {
        return -1;
    }
    
    // 启动显示
    temp_display_start(&config);
    
    // 清理资源
    temp_display_cleanup();
    
    return 0;
}
