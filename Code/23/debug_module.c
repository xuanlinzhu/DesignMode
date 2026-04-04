#include "modules.h"

#include <stdio.h>

#include "mediator.h"

void debug_module_init(debug_module_t *self, instrument_mediator_t *mediator)
{
    if (self == 0) {
        return;
    }
    self->mediator = mediator;
}

void debug_module_enter(debug_module_t *self)
{
    if (self == 0 || self->mediator == 0) {
        return;
    }
    printf("[DebugModule] request enter debug\n");
    mediator_notify(self->mediator, self, EVENT_ENTER_DEBUG);
}

void debug_module_exit(debug_module_t *self)
{
    if (self == 0 || self->mediator == 0) {
        return;
    }
    printf("[DebugModule] request exit debug\n");
    mediator_notify(self->mediator, self, EVENT_EXIT_DEBUG);
}
