CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = observer_demo
SOURCES = main.c subject.c display_observer.c alarm_observer.c logger_observer.c comm_observer.c
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    CLEAN_CMD = if exist main.o del /Q main.o & if exist subject.o del /Q subject.o & if exist display_observer.o del /Q display_observer.o & if exist alarm_observer.o del /Q alarm_observer.o & if exist logger_observer.o del /Q logger_observer.o & if exist comm_observer.o del /Q comm_observer.o & if exist $(TARGET) del /Q $(TARGET)
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
