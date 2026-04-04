#include "filter_strategy.h"

extern int printf(const char *format, ...);

static int average_process(filter_strategy_t *base, int input)
{
    int i;
    int sum;
    int output;
    average_strategy_t *self;

    if (base == 0) {
        return input;
    }

    self = (average_strategy_t *)base;

    self->buffer[self->index] = input;
    self->index = (self->index + 1) % AVG_WINDOW_SIZE;
    if (self->count < AVG_WINDOW_SIZE) {
        self->count += 1;
    }

    sum = 0;
    for (i = 0; i < self->count; ++i)
    {
        sum += self->buffer[i];
    }

    output = sum / self->count;
    printf("[AVG] input=%d output=%d\n", input, output);
    return output;
}

void average_strategy_init(average_strategy_t *self)
{
    int i;

    if (self == 0) {
        return;
    }

    self->base.name = "AVG";
    self->base.process = average_process;
    self->index = 0;
    self->count = 0;

    for (i = 0; i < AVG_WINDOW_SIZE; ++i)
    {
        self->buffer[i] = 0;
    }
}
