#include "modules.h"

#include <stdio.h>

void led_module_init(led_module_t *self, instrument_mediator_t *mediator)
{
    if (self == 0) {
        return;
    }
    self->mediator = mediator;
}

void led_set_default(led_module_t *self)
{
    if (self == 0) {
        return;
    }
    printf("[LED] DEFAULT\n");
}

void led_set_red_blink(led_module_t *self)
{
    if (self == 0) {
        return;
    }
    printf("[LED] RED BLINK\n");
}

void led_set_yellow_on(led_module_t *self)
{
    if (self == 0) {
        return;
    }
    printf("[LED] YELLOW ON\n");
}

void led_set_green_on(led_module_t *self)
{
    if (self == 0) {
        return;
    }
    printf("[LED] GREEN ON\n");
}
