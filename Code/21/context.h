#ifndef CONTEXT_H
#define CONTEXT_H

#define MAX_VARS 16
#define MAX_NAME_LEN 32

/* 固定容量变量表条目。 */
typedef struct
{
    char name[MAX_NAME_LEN];
    int value;
    int used;
} var_entry_t;

/* 解释器上下文：只用固定数组，不做动态扩容。 */
typedef struct
{
    var_entry_t vars[MAX_VARS];
} context_t;

void context_init(context_t *ctx);
int context_set_var(context_t *ctx, const char *name, int value);
int context_get_var(context_t *ctx, const char *name, int *out_value);
int context_del_var(context_t *ctx, const char *name);
int context_find_var(context_t *ctx, const char *name);

#endif
