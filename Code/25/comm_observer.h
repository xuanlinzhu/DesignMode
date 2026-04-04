#ifndef COMM_OBSERVER_H
#define COMM_OBSERVER_H

#include "observer.h"

typedef struct
{
    observer_t base;
} comm_observer_t;

void comm_observer_init(comm_observer_t *self);

#endif
