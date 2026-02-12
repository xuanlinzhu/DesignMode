# 智能温控器 - 抽象工厂模式演示
# Makefile for Windows (MinGW) and Linux/Unix

# 编译器
CC = gcc
CFLAGS = -Wall -Wextra -I.
LDFLAGS =

# Windows 平台检测
ifeq ($(OS),Windows_NT)
    TARGET = thermostat.exe
    LDFLAGS += -lwinmm
    RM_CMD = rm -f
else
    TARGET = thermostat
    RM_CMD = rm -f
endif

# 目录
BASIC_DIR = Basic
STANDARD_DIR = Standard
PRO_DIR = Pro
FACTORY_DIR = factory

# 源文件
SRCS = main.c \
       $(FACTORY_DIR)/FactoryRegistry.c \
       $(BASIC_DIR)/BasicFactory.c \
       $(BASIC_DIR)/LedDisplay.c \
       $(BASIC_DIR)/AnalogTempSensor.c \
       $(BASIC_DIR)/ButtonControl.c \
       $(BASIC_DIR)/FixedFan.c \
       $(STANDARD_DIR)/StandardFactory.c \
       $(STANDARD_DIR)/SevenSegDisplay.c \
       $(STANDARD_DIR)/DigitalTempSensor.c \
       $(STANDARD_DIR)/UartComm.c \
       $(STANDARD_DIR)/PwmFan.c \
       $(PRO_DIR)/ProFactory.c \
       $(PRO_DIR)/LcdDisplay.c \
       $(PRO_DIR)/HighPrecTempSensor.c \
       $(PRO_DIR)/WifiComm.c \
       $(PRO_DIR)/HighPerfFan.c

# 对象文件
OBJS = $(SRCS:.c=.o)

# 默认目标
.PHONY: all clean rebuild run

all: $(TARGET)

# 链接
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "编译完成: $(TARGET)"

# 编译规则
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理
clean:
	$(RM_CMD) $(OBJS) $(TARGET)
	@echo "清理完成"

# 重新编译
rebuild: clean all

# 运行
run: $(TARGET)
	./$(TARGET)

# 打印源文件列表（调试用）
print:
	@echo "源文件:"
	@echo $(SRCS)
	@echo "对象文件:"
	@echo $(OBJS)
