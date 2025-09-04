# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lwiringPi 

# 源文件
SRCS = main.c \
       components/botton.c components/clock.c components/beep.c components/rgb.c components/DHT.c components/usonic.c \
       combo/alarm_clock.c combo/stopwatch.c combo/rgb_control.c combo/temp_display.c
OBJS = $(addprefix target/,$(notdir $(SRCS:.c=.o)))
TARGET = main_app

# 包含目录
INCLUDES = -Icomponents -Icombo

# 默认目标
all: target_dir $(TARGET)

# 创建目标目录
target_dir:
	@mkdir -p target

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

target/%.o: components/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

target/%.o: combo/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

target/main.o: main.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 清理
clean:
	rm -f $(TARGET) target/*.o
	rmdir target 2>/dev/null || true

# 重新编译
rebuild: clean all

.PHONY: all clean rebuild target_dir
