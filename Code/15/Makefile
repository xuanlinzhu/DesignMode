CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -I.

TARGET = decorator_demo
SOURCES = main.c file_log_writer.c log_decorator.c timestamp_decorator.c level_decorator.c module_decorator.c
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
