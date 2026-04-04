CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = mediator_demo
SOURCES = main.c mediator.c alarm_module.c debug_module.c run_module.c display_module.c buzzer_module.c led_module.c
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    CLEAN_CMD = if exist main.o del /Q main.o & if exist mediator.o del /Q mediator.o & if exist alarm_module.o del /Q alarm_module.o & if exist debug_module.o del /Q debug_module.o & if exist run_module.o del /Q run_module.o & if exist display_module.o del /Q display_module.o & if exist buzzer_module.o del /Q buzzer_module.o & if exist led_module.o del /Q led_module.o & if exist $(TARGET) del /Q $(TARGET)
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
