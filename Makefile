# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lwiringPi 

# 源文件
SRCS = main.c \
       components/botton.c components/clock.c components/beep.c components/rgb.c components/DHT.c \
       combo/alarm_clock.c combo/stopwatch.c combo/rgb_control.c combo/temp_display.c
OBJS = $(addprefix target/,$(notdir $(SRCS:.c=.o)))
TARGET = main_app

# 单独的程序
TEMP_DISPLAY_SRCS = combo/temp_display.c components/DHT.c components/clock.c
TEMP_DISPLAY_OBJS = $(addprefix target/,$(notdir $(TEMP_DISPLAY_SRCS:.c=.o)))
TEMP_DISPLAY_TARGET = temp_display

# 包含目录
INCLUDES = -Icomponents -Icombo

# 默认目标
all: target_dir $(TARGET) $(TEMP_DISPLAY_TARGET)

# 创建目标目录
target_dir:
	@mkdir -p target

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(TEMP_DISPLAY_TARGET): $(TEMP_DISPLAY_OBJS)
	$(CC) $(CFLAGS) -o $@ $(TEMP_DISPLAY_OBJS) $(LDFLAGS)

target/%.o: components/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

target/%.o: combo/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

target/main.o: main.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 清理
clean:
	rm -f $(TARGET) $(TEMP_DISPLAY_TARGET) target/*.o
	rmdir target 2>/dev/null || true

# 重新编译
rebuild: clean all

# 单独编译温度显示程序
temp: target_dir $(TEMP_DISPLAY_TARGET)

.PHONY: all temp clean rebuild target_dir
