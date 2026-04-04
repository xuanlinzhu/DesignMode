CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = command_demo
SOURCES = main.c command_queue.c receiver.c command_pool.c led_on_command.c led_off_command.c motor_set_speed_command.c save_config_command.c
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    CLEAN_CMD = if exist main.o del /Q main.o & if exist command_queue.o del /Q command_queue.o & if exist receiver.o del /Q receiver.o & if exist command_pool.o del /Q command_pool.o & if exist led_on_command.o del /Q led_on_command.o & if exist led_off_command.o del /Q led_off_command.o & if exist motor_set_speed_command.o del /Q motor_set_speed_command.o & if exist save_config_command.o del /Q save_config_command.o & if exist $(TARGET) del /Q $(TARGET)
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
