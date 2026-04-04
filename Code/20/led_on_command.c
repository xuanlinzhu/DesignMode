#include "led_on_command.h"

#include "command_pool.h"

static void led_on_execute(command_t *base)
{
    led_on_command_t *self = (led_on_command_t *)base;

    if (self == 0 || self->receiver == 0) {
        return;
    }
    led_receiver_on(self->receiver);
}

static void led_on_destroy(command_t *base)
{
    command_pool_release(base);
}

void led_on_command_init(led_on_command_t *self, led_receiver_t *receiver)
{
    if (self == 0) {
        return;
    }

    self->base.execute = led_on_execute;
    self->base.destroy = led_on_destroy;
    self->base.name = "LedOnCommand";
    self->receiver = receiver;
}
