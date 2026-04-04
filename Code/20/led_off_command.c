#include "led_off_command.h"

#include "command_pool.h"

static void led_off_execute(command_t *base)
{
    led_off_command_t *self = (led_off_command_t *)base;

    if (self == 0 || self->receiver == 0) {
        return;
    }
    led_receiver_off(self->receiver);
}

static void led_off_destroy(command_t *base)
{
    command_pool_release(base);
}

void led_off_command_init(led_off_command_t *self, led_receiver_t *receiver)
{
    if (self == 0) {
        return;
    }

    self->base.execute = led_off_execute;
    self->base.destroy = led_off_destroy;
    self->base.name = "LedOffCommand";
    self->receiver = receiver;
}
