#include "expression.h"

#define MAX_SUB_EXPR 32

typedef struct
{
    expression_t base;
    int used;
    expression_t *left;
    expression_t *right;
} sub_expression_t;

static sub_expression_t g_pool[MAX_SUB_EXPR];

static int sub_interpret(expression_t *base, context_t *ctx, int *out_value)
{
    sub_expression_t *self = (sub_expression_t *)base;
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

    *out_value = left_value - right_value;
    return 1;
}

static void sub_destroy(expression_t *base)
{
    sub_expression_t *self = (sub_expression_t *)base;

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

expression_t *sub_expression_create(expression_t *left, expression_t *right)
{
    int i;

    if (left == 0 || right == 0) {
        return 0;
    }

    for (i = 0; i < MAX_SUB_EXPR; i++) {
        if (!g_pool[i].used) {
            g_pool[i].used = 1;
            g_pool[i].base.interpret = sub_interpret;
            g_pool[i].base.destroy = sub_destroy;
            g_pool[i].left = left;
            g_pool[i].right = right;
            return &g_pool[i].base;
        }
    }
    return 0;
}
