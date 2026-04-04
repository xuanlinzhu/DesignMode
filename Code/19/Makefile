CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = chain_demo
SOURCES = main.c handler.c blacklist_handler.c connlimit_handler.c ratelimit_handler.c auth_handler.c
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    CLEAN_CMD = if exist main.o del /Q main.o & if exist handler.o del /Q handler.o & if exist blacklist_handler.o del /Q blacklist_handler.o & if exist connlimit_handler.o del /Q connlimit_handler.o & if exist ratelimit_handler.o del /Q ratelimit_handler.o & if exist auth_handler.o del /Q auth_handler.o & if exist $(TARGET) del /Q $(TARGET)
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
