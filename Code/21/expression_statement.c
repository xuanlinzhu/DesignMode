#include "statement.h"

#include <stdio.h>

#define MAX_EXPR_STMT 32

typedef struct
{
    statement_t base;
    int used;
    expression_t *expr;
} expression_statement_t;

static expression_statement_t g_pool[MAX_EXPR_STMT];

static int expression_stmt_execute(statement_t *base, context_t *ctx)
{
    expression_statement_t *self = (expression_statement_t *)base;
    int value;

    if (self == 0 || ctx == 0 || self->expr == 0) {
        return 0;
    }

    if (!self->expr->interpret(self->expr, ctx, &value)) {
        return 0;
    }

    printf("[Expr] result = %d\n", value);
    return 1;
}

static void expression_stmt_destroy(statement_t *base)
{
    expression_statement_t *self = (expression_statement_t *)base;

    if (self == 0) {
        return;
    }

    if (self->expr != 0) {
        self->expr->destroy(self->expr);
        self->expr = 0;
    }
    self->used = 0;
}

statement_t *expression_statement_create(expression_t *expr)
{
    int i;

    if (expr == 0) {
        return 0;
    }

    for (i = 0; i < MAX_EXPR_STMT; i++) {
        if (!g_pool[i].used) {
            g_pool[i].used = 1;
            g_pool[i].base.execute = expression_stmt_execute;
            g_pool[i].base.destroy = expression_stmt_destroy;
            g_pool[i].expr = expr;
            return &g_pool[i].base;
        }
    }
    return 0;
}
