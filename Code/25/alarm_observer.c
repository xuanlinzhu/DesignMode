#include "alarm_observer.h"

extern int printf(const char *format, ...);

static void alarm_observer_update(observer_t *base, int value)
{
    alarm_observer_t *self;

    if (base == 0) {
        return;
    }

    self = (alarm_observer_t *)base;

    if (value > self->threshold) {
        printf("[Alarm] value = %d, threshold = %d, alarm ON\n", value, self->threshold);
    }
    else {
        printf("[Alarm] value = %d, threshold = %d, normal\n", value, self->threshold);
    }
}

void alarm_observer_init(alarm_observer_t *self, int threshold)
{
    if (self == 0) {
        return;
    }

    self->base.update = alarm_observer_update;
    self->threshold = threshold;
}
