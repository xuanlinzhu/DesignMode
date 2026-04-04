#include "filter_strategy.h"

extern int printf(const char *format, ...);

void filter_context_init(filter_context_t *ctx, filter_strategy_t *strategy)
{
    if (ctx == 0) {
        return;
    }

    ctx->strategy = strategy;
    if (strategy != 0) {
        printf("[Context] init strategy = %s\n", strategy->name);
    }
}

void filter_set_strategy(filter_context_t *ctx, filter_strategy_t *strategy)
{
    if (ctx == 0 || strategy == 0) {
        return;
    }

    ctx->strategy = strategy;
    printf("[Context] switch strategy -> %s\n", strategy->name);
}

int filter_process(filter_context_t *ctx, int input)
{
    if (ctx == 0 || ctx->strategy == 0 || ctx->strategy->process == 0) {
        return input;
    }

    return ctx->strategy->process(ctx->strategy, input);
}
