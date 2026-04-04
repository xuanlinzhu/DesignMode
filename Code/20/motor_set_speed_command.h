#ifndef MOTOR_SET_SPEED_COMMAND_H
#define MOTOR_SET_SPEED_COMMAND_H

#include "command.h"
#include "receiver.h"

typedef struct
{
    command_t base;
    motor_receiver_t *receiver;
    int speed;
} motor_set_speed_command_t;

void motor_set_speed_command_init(motor_set_speed_command_t *self, motor_receiver_t *receiver, int speed);

#endif
