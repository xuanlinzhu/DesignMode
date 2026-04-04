#ifndef FILTER_STRATEGY_H
#define FILTER_STRATEGY_H

#define AVG_WINDOW_SIZE 3

typedef struct filter_strategy
{
    const char *name;
    int (*process)(struct filter_strategy *self, int input);
} filter_strategy_t;

typedef struct
{
    filter_strategy_t *strategy;
} filter_context_t;

typedef struct
{
    filter_strategy_t base;
} raw_strategy_t;

typedef struct
{
    filter_strategy_t base;
    int buffer[AVG_WINDOW_SIZE];
    int index;
    int count;
} average_strategy_t;

typedef struct
{
    filter_strategy_t base;
    int last_value;
    int threshold;
    int initialized;
} clamp_strategy_t;

void filter_context_init(filter_context_t *ctx, filter_strategy_t *strategy);
void filter_set_strategy(filter_context_t *ctx, filter_strategy_t *strategy);
int filter_process(filter_context_t *ctx, int input);

void raw_strategy_init(raw_strategy_t *self);
void average_strategy_init(average_strategy_t *self);
void clamp_strategy_init(clamp_strategy_t *self, int threshold);

#endif
