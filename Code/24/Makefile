CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = memento_demo
SOURCES = main.c instrument_param.c debug_caretaker.c default_caretaker.c
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    CLEAN_CMD = if exist main.o del /Q main.o & if exist instrument_param.o del /Q instrument_param.o & if exist debug_caretaker.o del /Q debug_caretaker.o & if exist default_caretaker.o del /Q default_caretaker.o & if exist $(TARGET) del /Q $(TARGET)
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
