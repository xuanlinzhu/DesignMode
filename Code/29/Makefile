CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = visitor_demo
SOURCES = main.c temp_sensor.c pressure_sensor.c motor.c info_visitor.c selftest_visitor.c
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    CLEAN_CMD = if exist main.o del /Q main.o & if exist temp_sensor.o del /Q temp_sensor.o & if exist pressure_sensor.o del /Q pressure_sensor.o & if exist motor.o del /Q motor.o & if exist info_visitor.o del /Q info_visitor.o & if exist selftest_visitor.o del /Q selftest_visitor.o & if exist $(TARGET) del /Q $(TARGET)
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
