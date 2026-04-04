#ifndef DISPLAY_OBSERVER_H
#define DISPLAY_OBSERVER_H

#include "observer.h"

typedef struct
{
    observer_t base;
} display_observer_t;

void display_observer_init(display_observer_t *self);

#endif
