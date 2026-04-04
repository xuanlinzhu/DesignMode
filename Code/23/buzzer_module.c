#include "modules.h"

#include <stdio.h>

void buzzer_module_init(buzzer_module_t *self, instrument_mediator_t *mediator)
{
    if (self == 0) {
        return;
    }
    self->mediator = mediator;
    self->on = 0;
}

void buzzer_on(buzzer_module_t *self)
{
    if (self == 0) {
        return;
    }
    self->on = 1;
    printf("[Buzzer] ON\n");
}

void buzzer_off(buzzer_module_t *self)
{
    if (self == 0) {
        return;
    }
    self->on = 0;
    printf("[Buzzer] OFF\n");
}
