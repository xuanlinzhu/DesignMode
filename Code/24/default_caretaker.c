#include "default_caretaker.h"

#include <stdio.h>

void default_caretaker_init(default_param_caretaker_t *self)
{
    if (self == 0) {
        return;
    }
    self->has_default_snapshot = 0;
}

void default_caretaker_save(default_param_caretaker_t *self, param_memento_t memento)
{
    if (self == 0) {
        return;
    }
    self->default_snapshot = memento;
    self->has_default_snapshot = 1;
    printf("[DefaultCaretaker] save default snapshot\n");
}

int default_caretaker_has_snapshot(const default_param_caretaker_t *self)
{
    if (self == 0) {
        return 0;
    }
    return self->has_default_snapshot;
}

const param_memento_t *default_caretaker_get(const default_param_caretaker_t *self)
{
    if (self == 0 || !self->has_default_snapshot) {
        return 0;
    }
    return &self->default_snapshot;
}
