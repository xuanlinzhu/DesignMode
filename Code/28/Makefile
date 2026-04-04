CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = template_method_demo
SOURCES = main.c measurement_processor.c normal_processor.c fast_processor.c precise_processor.c
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    CLEAN_CMD = if exist main.o del /Q main.o & if exist measurement_processor.o del /Q measurement_processor.o & if exist normal_processor.o del /Q normal_processor.o & if exist fast_processor.o del /Q fast_processor.o & if exist precise_processor.o del /Q precise_processor.o & if exist $(TARGET) del /Q $(TARGET)
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
