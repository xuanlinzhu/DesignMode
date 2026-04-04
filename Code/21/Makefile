CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = interpreter_demo
SOURCES = main.c context.c number_expression.c variable_expression.c add_expression.c sub_expression.c assign_statement.c print_statement.c delete_statement.c expression_statement.c parser.c
OBJECTS = $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    CLEAN_CMD = if exist main.o del /Q main.o & if exist context.o del /Q context.o & if exist number_expression.o del /Q number_expression.o & if exist variable_expression.o del /Q variable_expression.o & if exist add_expression.o del /Q add_expression.o & if exist sub_expression.o del /Q sub_expression.o & if exist assign_statement.o del /Q assign_statement.o & if exist print_statement.o del /Q print_statement.o & if exist delete_statement.o del /Q delete_statement.o & if exist expression_statement.o del /Q expression_statement.o & if exist parser.o del /Q parser.o & if exist $(TARGET) del /Q $(TARGET)
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
