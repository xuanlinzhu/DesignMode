#include "logger_observer.h"

extern int printf(const char *format, ...);

static void logger_observer_update(observer_t *base, int value)
{
    (void)base;
    printf("[Logger] record adc value: %d\n", value);
}

void logger_observer_init(logger_observer_t *self)
{
    if (self == 0) {
        return;
    }

    self->base.update = logger_observer_update;
}
