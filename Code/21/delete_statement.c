#include "statement.h"

#include <stdio.h>
#include <string.h>

#define MAX_DELETE_STMT 16

typedef struct
{
    statement_t base;
    int used;
    char name[MAX_NAME_LEN];
} delete_statement_t;

static delete_statement_t g_pool[MAX_DELETE_STMT];

static int delete_execute(statement_t *base, context_t *ctx)
{
    delete_statement_t *self = (delete_statement_t *)base;

    if (self == 0 || ctx == 0) {
        return 0;
    }

    if (!context_del_var(ctx, self->name)) {
        printf("[Error] delete failed, variable '%s' not found\n", self->name);
        return 0;
    }

    printf("[Delete] remove variable %s\n", self->name);
    return 1;
}

static void delete_destroy(statement_t *base)
{
    delete_statement_t *self = (delete_statement_t *)base;

    if (self == 0) {
        return;
    }
    self->used = 0;
    self->name[0] = '\0';
}

statement_t *delete_statement_create(const char *name)
{
    int i;

    if (name == 0 || name[0] == '\0') {
        return 0;
    }

    for (i = 0; i < MAX_DELETE_STMT; i++) {
        if (!g_pool[i].used) {
            g_pool[i].used = 1;
            g_pool[i].base.execute = delete_execute;
            g_pool[i].base.destroy = delete_destroy;
            strncpy(g_pool[i].name, name, MAX_NAME_LEN - 1);
            g_pool[i].name[MAX_NAME_LEN - 1] = '\0';
            return &g_pool[i].base;
        }
    }
    return 0;
}
