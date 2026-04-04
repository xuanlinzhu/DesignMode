CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = state_demo
SOURCES = main.c state_machine.c idle_state.c running_state.c alarm_state.c debug_state.c
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    CLEAN_CMD = if exist main.o del /Q main.o & if exist state_machine.o del /Q state_machine.o & if exist idle_state.o del /Q idle_state.o & if exist running_state.o del /Q running_state.o & if exist alarm_state.o del /Q alarm_state.o & if exist debug_state.o del /Q debug_state.o & if exist $(TARGET) del /Q $(TARGET)
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
