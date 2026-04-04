# 编译器
CC = gcc

# 编译选项
CFLAGS = -Wall -O2

# 目标程序名
TARGET = prototype_control_demo

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): prototype_control_demo.c
	$(CC) $(CFLAGS) -o $(TARGET) prototype_control_demo.c

# 清理生成文件
clean:
	rm -f $(TARGET)