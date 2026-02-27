CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -I.

TARGET = driver_factory_demo
SOURCES = main.c dev.c bus.c spi_core.c spi_flash_drv.c spi_lcd_drv.c uart_drv.c
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
endif

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(TARGET)

.PHONY: all clean
