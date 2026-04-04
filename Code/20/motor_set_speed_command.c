#include "motor_set_speed_command.h"

#include "command_pool.h"

static void motor_set_speed_execute(command_t *base)
{
    motor_set_speed_command_t *self = (motor_set_speed_command_t *)base;

    if (self == 0 || self->receiver == 0) {
        return;
    }
    motor_receiver_set_speed(self->receiver, self->speed);
}

static void motor_set_speed_destroy(command_t *base)
{
    command_pool_release(base);
}

void motor_set_speed_command_init(motor_set_speed_command_t *self, motor_receiver_t *receiver, int speed)
{
    if (self == 0) {
        return;
    }

    self->base.execute = motor_set_speed_execute;
    self->base.destroy = motor_set_speed_destroy;
    self->base.name = "MotorSetSpeedCommand";
    self->receiver = receiver;
    self->speed = speed;
}
