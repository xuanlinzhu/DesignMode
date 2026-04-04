#ifndef LED_OFF_COMMAND_H
#define LED_OFF_COMMAND_H

#include "command.h"
#include "receiver.h"

typedef struct
{
    command_t base;
    led_receiver_t *receiver;
} led_off_command_t;

void led_off_command_init(led_off_command_t *self, led_receiver_t *receiver);

#endif
