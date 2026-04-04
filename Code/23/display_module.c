#include "modules.h"

#include <stdio.h>

void display_module_init(display_module_t *self, instrument_mediator_t *mediator)
{
    if (self == 0) {
        return;
    }
    self->mediator = mediator;
}

void display_show_main(display_module_t *self)
{
    if (self == 0) {
        return;
    }
    printf("[Display] show main page\n");
}

void display_show_alarm(display_module_t *self)
{
    if (self == 0) {
        return;
    }
    printf("[Display] show alarm page\n");
}

void display_show_debug(display_module_t *self)
{
    if (self == 0) {
        return;
    }
    printf("[Display] show debug page\n");
}

void display_show_run(display_module_t *self)
{
    if (self == 0) {
        return;
    }
    printf("[Display] show run page\n");
}
