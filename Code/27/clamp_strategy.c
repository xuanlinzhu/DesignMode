#include "filter_strategy.h"

extern int printf(const char *format, ...);

static int clamp_process(filter_strategy_t *base, int input)
{
    int output;
    clamp_strategy_t *self;

    if (base == 0) {
        return input;
    }

    self = (clamp_strategy_t *)base;

    if (!self->initialized)
    {
        self->last_value = input;
        self->initialized = 1;
        printf("[CLAMP] input=%d output=%d (init)\n", input, input);
        return input;
    }

    output = input;
    if (input - self->last_value > self->threshold) {
        output = self->last_value + self->threshold;
    }
    else if (self->last_value - input > self->threshold) {
        output = self->last_value - self->threshold;
    }

    self->last_value = output;
    printf("[CLAMP] input=%d output=%d\n", input, output);
    return output;
}

void clamp_strategy_init(clamp_strategy_t *self, int threshold)
{
    if (self == 0) {
        return;
    }

    self->base.name = "CLAMP";
    self->base.process = clamp_process;
    self->last_value = 0;
    self->threshold = threshold;
    self->initialized = 0;
}
