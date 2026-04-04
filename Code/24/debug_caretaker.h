#ifndef DEBUG_CARETAKER_H
#define DEBUG_CARETAKER_H

#include "param_memento.h"

typedef struct
{
    param_memento_t debug_snapshot;
    int has_debug_snapshot;
} debug_param_caretaker_t;

void debug_caretaker_init(debug_param_caretaker_t *self);
void debug_caretaker_save(debug_param_caretaker_t *self, param_memento_t memento);
int debug_caretaker_has_snapshot(const debug_param_caretaker_t *self);
const param_memento_t *debug_caretaker_get(const debug_param_caretaker_t *self);
void debug_caretaker_clear(debug_param_caretaker_t *self);

#endif
