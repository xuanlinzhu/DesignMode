#ifndef LOGGER_OBSERVER_H
#define LOGGER_OBSERVER_H

#include "observer.h"

typedef struct
{
    observer_t base;
} logger_observer_t;

void logger_observer_init(logger_observer_t *self);

#endif
