CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = strategy_demo
SOURCES = main.c filter_context.c raw_strategy.c average_strategy.c clamp_strategy.c
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    CLEAN_CMD = if exist main.o del /Q main.o & if exist filter_context.o del /Q filter_context.o & if exist raw_strategy.o del /Q raw_strategy.o & if exist average_strategy.o del /Q average_strategy.o & if exist clamp_strategy.o del /Q clamp_strategy.o & if exist $(TARGET) del /Q $(TARGET)
else
    CLEAN_CMD = $(RM) $(OBJECTS) $(TARGET)
endif

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	$(CLEAN_CMD)
