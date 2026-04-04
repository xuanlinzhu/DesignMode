#include "statement.h"

#include <stdio.h>
#include <string.h>

#define MAX_PRINT_STMT 16

typedef struct
{
    statement_t base;
    int used;
    char name[MAX_NAME_LEN];
} print_statement_t;

static print_statement_t g_pool[MAX_PRINT_STMT];

static int print_execute(statement_t *base, context_t *ctx)
{
    print_statement_t *self = (print_statement_t *)base;
    int value;

    if (self == 0 || ctx == 0) {
        return 0;
    }

    if (!context_get_var(ctx, self->name, &value)) {
        printf("[Error] variable '%s' not found\n", self->name);
        return 0;
    }

    printf("[Print] %s = %d\n", self->name, value);
    return 1;
}

static void print_destroy(statement_t *base)
{
    print_statement_t *self = (print_statement_t *)base;

    if (self == 0) {
        return;
    }
    self->used = 0;
    self->name[0] = '\0';
}

statement_t *print_statement_create(const char *name)
{
    int i;

    if (name == 0 || name[0] == '\0') {
        return 0;
    }

    for (i = 0; i < MAX_PRINT_STMT; i++) {
        if (!g_pool[i].used) {
            g_pool[i].used = 1;
            g_pool[i].base.execute = print_execute;
            g_pool[i].base.destroy = print_destroy;
            strncpy(g_pool[i].name, name, MAX_NAME_LEN - 1);
            g_pool[i].name[MAX_NAME_LEN - 1] = '\0';
            return &g_pool[i].base;
        }
    }
    return 0;
}
