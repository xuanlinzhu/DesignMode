# Builder 模式 - 多层嵌套协议构造示例
# Makefile for Windows (MinGW) and Linux/Unix

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -I.
LDFLAGS =

ifeq ($(OS),Windows_NT)
    TARGET = protocol_builder_demo.exe
    RM_CMD = rm -f
else
    TARGET = protocol_builder_demo
    RM_CMD = rm -f
endif

SRCS = main.c \
       protocol_builder.c

OBJS = $(SRCS:.c=.o)

.PHONY: all clean rebuild run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM_CMD) $(OBJS) $(TARGET)
	@echo "Clean complete"

rebuild: clean all

run: $(TARGET)
	./$(TARGET)
