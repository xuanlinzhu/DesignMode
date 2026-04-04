#ifndef PARSER_H
#define PARSER_H

#include "statement.h"

statement_t *parse_statement(const char *line);
expression_t *parse_expression(const char *text);

#endif
