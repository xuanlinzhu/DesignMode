#include "modules.h"

#include <stdio.h>

#include "mediator.h"

void alarm_module_init(alarm_module_t *self, instrument_mediator_t *mediator)
{
    if (self == 0) {
        return;
    }
    self->mediator = mediator;
}

void alarm_module_trigger(alarm_module_t *self)
{
    if (self == 0 || self->mediator == 0) {
        return;
    }
    printf("[AlarmModule] trigger alarm\n");
    mediator_notify(self->mediator, self, EVENT_ALARM_ON);
}

void alarm_module_clear(alarm_module_t *self)
{
    if (self == 0 || self->mediator == 0) {
        return;
    }
    printf("[AlarmModule] clear alarm\n");
    mediator_notify(self->mediator, self, EVENT_ALARM_OFF);
}

void alarm_module_manual_trigger(alarm_module_t *self)
{
    if (self == 0 || self->mediator == 0) {
        return;
    }
    printf("[AlarmModule] manual trigger alarm\n");
    mediator_notify(self->mediator, self, EVENT_MANUAL_ALARM_ON);
}
