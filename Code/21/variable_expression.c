#include "expression.h"

#include <stdio.h>
#include <string.h>

#define MAX_VAR_EXPR 32

typedef struct
{
    expression_t base;
    int used;
    char name[MAX_NAME_LEN];
} variable_expression_t;

static variable_expression_t g_pool[MAX_VAR_EXPR];

static int variable_interpret(expression_t *base, context_t *ctx, int *out_value)
{
    variable_expression_t *self = (variable_expression_t *)base;

    if (self == 0 || ctx == 0 || out_value == 0) {
        return 0;
    }

    if (!context_get_var(ctx, self->name, out_value)) {
        printf("[Error] variable '%s' not found\n", self->name);
        return 0;
    }
    return 1;
}

static void variable_destroy(expression_t *base)
{
    variable_expression_t *self = (variable_expression_t *)base;

    if (self == 0) {
        return;
    }
    self->used = 0;
    self->name[0] = '\0';
}

expression_t *variable_expression_create(const char *name)
{
    int i;

    if (name == 0 || name[0] == '\0') {
        return 0;
    }

    for (i = 0; i < MAX_VAR_EXPR; i++) {
        if (!g_pool[i].used) {
            g_pool[i].used = 1;
            g_pool[i].base.interpret = variable_interpret;
            g_pool[i].base.destroy = variable_destroy;
            strncpy(g_pool[i].name, name, MAX_NAME_LEN - 1);
            g_pool[i].name[MAX_NAME_LEN - 1] = '\0';
            return &g_pool[i].base;
        }
    }
    return 0;
}
