#include "comm_observer.h"

extern int printf(const char *format, ...);

static void comm_observer_update(observer_t *base, int value)
{
    (void)base;
    printf("[Comm] upload adc value: %d\n", value);
}

void comm_observer_init(comm_observer_t *self)
{
    if (self == 0) {
        return;
    }

    self->base.update = comm_observer_update;
}
