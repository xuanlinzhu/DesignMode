#include "filter_strategy.h"

extern int printf(const char *format, ...);

static int raw_process(filter_strategy_t *base, int input)
{
    (void)base;
    printf("[RAW] input=%d output=%d\n", input, input);
    return input;
}

void raw_strategy_init(raw_strategy_t *self)
{
    if (self == 0) {
        return;
    }

    self->base.name = "RAW";
    self->base.process = raw_process;
}
