#include "debug_caretaker.h"

#include <stdio.h>

void debug_caretaker_init(debug_param_caretaker_t *self)
{
    if (self == 0) {
        return;
    }
    self->has_debug_snapshot = 0;
}

void debug_caretaker_save(debug_param_caretaker_t *self, param_memento_t memento)
{
    if (self == 0) {
        return;
    }
    self->debug_snapshot = memento;
    self->has_debug_snapshot = 1;
    printf("[DebugCaretaker] save debug snapshot\n");
}

int debug_caretaker_has_snapshot(const debug_param_caretaker_t *self)
{
    if (self == 0) {
        return 0;
    }
    return self->has_debug_snapshot;
}

const param_memento_t *debug_caretaker_get(const debug_param_caretaker_t *self)
{
    if (self == 0 || !self->has_debug_snapshot) {
        return 0;
    }
    return &self->debug_snapshot;
}

void debug_caretaker_clear(debug_param_caretaker_t *self)
{
    if (self == 0) {
        return;
    }
    self->has_debug_snapshot = 0;
    printf("[DebugCaretaker] clear debug snapshot\n");
}
