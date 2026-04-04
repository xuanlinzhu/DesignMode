#ifndef STATEMENT_H
#define STATEMENT_H

#include "context.h"
#include "expression.h"

/* 抽象语句接口：统一执行与销毁。 */
typedef struct statement
{
    int (*execute)(struct statement *self, context_t *ctx);
    void (*destroy)(struct statement *self);
} statement_t;

statement_t *assign_statement_create(const char *name, expression_t *expr);
statement_t *print_statement_create(const char *name);
statement_t *delete_statement_create(const char *name);
statement_t *expression_statement_create(expression_t *expr);

#endif
