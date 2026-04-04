#include "statement.h"

#include <stdio.h>
#include <string.h>

#define MAX_ASSIGN_STMT 32

typedef struct
{
    statement_t base;
    int used;
    char name[MAX_NAME_LEN];
    expression_t *expr;
} assign_statement_t;

static assign_statement_t g_pool[MAX_ASSIGN_STMT];

static int assign_execute(statement_t *base, context_t *ctx)
{
    assign_statement_t *self = (assign_statement_t *)base;
    int value;
    int existed;

    if (self == 0 || ctx == 0 || self->expr == 0) {
        return 0;
    }

    if (!self->expr->interpret(self->expr, ctx, &value)) {
        return 0;
    }

    existed = (context_find_var(ctx, self->name) >= 0);
    if (!context_set_var(ctx, self->name, value)) {
        printf("[Error] variable table full, cannot set '%s'\n", self->name);
        return 0;
    }

    if (existed) {
        printf("[Assign] update variable %s = %d\n", self->name, value);
    } else {
        printf("[Assign] create variable %s = %d\n", self->name, value);
    }
    return 1;
}

static void assign_destroy(statement_t *base)
{
    assign_statement_t *self = (assign_statement_t *)base;

    if (self == 0) {
        return;
    }
    if (self->expr != 0) {
        self->expr->destroy(self->expr);
        self->expr = 0;
    }
    self->used = 0;
    self->name[0] = '\0';
}

statement_t *assign_statement_create(const char *name, expression_t *expr)
{
    int i;

    if (name == 0 || name[0] == '\0' || expr == 0) {
        return 0;
    }

    for (i = 0; i < MAX_ASSIGN_STMT; i++) {
        if (!g_pool[i].used) {
            g_pool[i].used = 1;
            g_pool[i].base.execute = assign_execute;
            g_pool[i].base.destroy = assign_destroy;
            strncpy(g_pool[i].name, name, MAX_NAME_LEN - 1);
            g_pool[i].name[MAX_NAME_LEN - 1] = '\0';
            g_pool[i].expr = expr;
            return &g_pool[i].base;
        }
    }
    return 0;
}
