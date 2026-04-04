CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = iterator_demo
SOURCES = main.c ram_log_storage.c ram_log_iterator.c flash_log_storage.c flash_log_iterator.c log_viewer.c
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    CLEAN_CMD = if exist main.o del /Q main.o & if exist ram_log_storage.o del /Q ram_log_storage.o & if exist ram_log_iterator.o del /Q ram_log_iterator.o & if exist flash_log_storage.o del /Q flash_log_storage.o & if exist flash_log_iterator.o del /Q flash_log_iterator.o & if exist log_viewer.o del /Q log_viewer.o & if exist $(TARGET) del /Q $(TARGET)
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
