# 编译器与基础编译选项
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -I.
LDFLAGS = -pthread

TARGET = singleton_demo
SOURCES = demo.c glog.c log_driver_console.c
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
endif

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(TARGET)

.PHONY: all clean
