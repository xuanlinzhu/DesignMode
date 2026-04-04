#include "context.h"

#include <string.h>

void context_init(context_t *ctx)
{
    int i;

    if (ctx == 0) {
        return;
    }

    for (i = 0; i < MAX_VARS; i++) {
        ctx->vars[i].name[0] = '\0';
        ctx->vars[i].value = 0;
        ctx->vars[i].used = 0;
    }
}

int context_find_var(context_t *ctx, const char *name)
{
    int i;

    if (ctx == 0 || name == 0) {
        return -1;
    }

    for (i = 0; i < MAX_VARS; i++) {
        if (ctx->vars[i].used && strcmp(ctx->vars[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int context_set_var(context_t *ctx, const char *name, int value)
{
    int i;
    int idx;

    if (ctx == 0 || name == 0 || name[0] == '\0') {
        return 0;
    }

    idx = context_find_var(ctx, name);
    if (idx >= 0) {
        ctx->vars[idx].value = value;
        return 1;
    }

    for (i = 0; i < MAX_VARS; i++) {
        if (!ctx->vars[i].used) {
            ctx->vars[i].used = 1;
            strncpy(ctx->vars[i].name, name, MAX_NAME_LEN - 1);
            ctx->vars[i].name[MAX_NAME_LEN - 1] = '\0';
            ctx->vars[i].value = value;
            return 1;
        }
    }

    return 0;
}

int context_get_var(context_t *ctx, const char *name, int *out_value)
{
    int idx;

    if (ctx == 0 || name == 0 || out_value == 0) {
        return 0;
    }

    idx = context_find_var(ctx, name);
    if (idx < 0) {
        return 0;
    }

    *out_value = ctx->vars[idx].value;
    return 1;
}

int context_del_var(context_t *ctx, const char *name)
{
    int idx;

    if (ctx == 0 || name == 0) {
        return 0;
    }

    idx = context_find_var(ctx, name);
    if (idx < 0) {
        return 0;
    }

    ctx->vars[idx].used = 0;
    ctx->vars[idx].name[0] = '\0';
    ctx->vars[idx].value = 0;
    return 1;
}
