CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -I. -I./facade -I./subsystem -I./common

TARGET = power_demo
SOURCES = main.c facade/PowerFacade.c subsystem/ClockManager.c subsystem/VoltageManager.c subsystem/PeripheralManager.c subsystem/ThermalManager.c subsystem/SchedulerManager.c
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
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
	$(RM) $(OBJECTS) $(TARGET)
