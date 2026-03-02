CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -I.

TARGET = bridge_demo
SOURCES = app_demo.c phy_uart.c phy_tcp.c protocol_modbus.c protocol_file_transfer.c
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
