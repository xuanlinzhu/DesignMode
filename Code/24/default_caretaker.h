#ifndef DEFAULT_CARETAKER_H
#define DEFAULT_CARETAKER_H

#include "param_memento.h"

typedef struct
{
    param_memento_t default_snapshot;
    int has_default_snapshot;
} default_param_caretaker_t;

void default_caretaker_init(default_param_caretaker_t *self);
void default_caretaker_save(default_param_caretaker_t *self, param_memento_t memento);
int default_caretaker_has_snapshot(const default_param_caretaker_t *self);
const param_memento_t *default_caretaker_get(const default_param_caretaker_t *self);

#endif
