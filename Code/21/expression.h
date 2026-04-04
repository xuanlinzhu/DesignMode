#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "context.h"

/* 抽象表达式接口：统一解释与销毁入口。 */
typedef struct expression
{
    int (*interpret)(struct expression *self, context_t *ctx, int *out_value);
    void (*destroy)(struct expression *self);
} expression_t;

expression_t *number_expression_create(int value);
expression_t *variable_expression_create(const char *name);
expression_t *add_expression_create(expression_t *left, expression_t *right);
expression_t *sub_expression_create(expression_t *left, expression_t *right);

#endif
