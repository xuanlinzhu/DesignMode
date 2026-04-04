#include "expression.h"

#define MAX_ADD_EXPR 32

typedef struct
{
    expression_t base;
    int used;
    expression_t *left;
    expression_t *right;
} add_expression_t;

static add_expression_t g_pool[MAX_ADD_EXPR];

static int add_interpret(expression_t *base, context_t *ctx, int *out_value)
{
    add_expression_t *self = (add_expression_t *)base;
    int left_value;
    int right_value;

    if (self == 0 || ctx == 0 || out_value == 0 || self->left == 0 || self->right == 0) {
        return 0;
    }

    if (!self->left->interpret(self->left, ctx, &left_value)) {
        return 0;
    }
    if (!self->right->interpret(self->right, ctx, &right_value)) {
        return 0;
    }

    *out_value = left_value + right_value;
    return 1;
}

static void add_destroy(expression_t *base)
{
    add_expression_t *self = (add_expression_t *)base;

    if (self == 0) {
        return;
    }

    if (self->left != 0) {
        self->left->destroy(self->left);
        self->left = 0;
    }
    if (self->right != 0) {
        self->right->destroy(self->right);
        self->right = 0;
    }
    self->used = 0;
}

expression_t *add_expression_create(expression_t *left, expression_t *right)
{
    int i;

    if (left == 0 || right == 0) {
        return 0;
    }

    for (i = 0; i < MAX_ADD_EXPR; i++) {
        if (!g_pool[i].used) {
            g_pool[i].used = 1;
            g_pool[i].base.interpret = add_interpret;
            g_pool[i].base.destroy = add_destroy;
            g_pool[i].left = left;
            g_pool[i].right = right;
            return &g_pool[i].base;
        }
    }
    return 0;
}
