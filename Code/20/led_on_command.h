#ifndef LED_ON_COMMAND_H
#define LED_ON_COMMAND_H

#include "command.h"
#include "receiver.h"

typedef struct
{
    command_t base;
    led_receiver_t *receiver;
} led_on_command_t;

void led_on_command_init(led_on_command_t *self, led_receiver_t *receiver);

#endif
