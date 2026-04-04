#include "expression.h"

#define MAX_NUMBER_EXPR 32

typedef struct
{
    expression_t base;
    int used;
    int value;
} number_expression_t;

static number_expression_t g_pool[MAX_NUMBER_EXPR];

static int number_interpret(expression_t *base, context_t *ctx, int *out_value)
{
    number_expression_t *self = (number_expression_t *)base;
    (void)ctx;

    if (self == 0 || out_value == 0) {
        return 0;
    }

    *out_value = self->value;
    return 1;
}

static void number_destroy(expression_t *base)
{
    number_expression_t *self = (number_expression_t *)base;

    if (self == 0) {
        return;
    }
    self->used = 0;
}

expression_t *number_expression_create(int value)
{
    int i;

    for (i = 0; i < MAX_NUMBER_EXPR; i++) {
        if (!g_pool[i].used) {
            g_pool[i].used = 1;
            g_pool[i].base.interpret = number_interpret;
            g_pool[i].base.destroy = number_destroy;
            g_pool[i].value = value;
            return &g_pool[i].base;
        }
    }
    return 0;
}
