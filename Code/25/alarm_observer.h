#ifndef ALARM_OBSERVER_H
#define ALARM_OBSERVER_H

#include "observer.h"

typedef struct
{
    observer_t base;
    int threshold;
} alarm_observer_t;

void alarm_observer_init(alarm_observer_t *self, int threshold);

#endif
