#include "receiver.h"

extern int printf(const char *format, ...);

void led_receiver_init(led_receiver_t *self)
{
    if (self == 0) {
        return;
    }
    self->is_on = 0;
}

void led_receiver_on(led_receiver_t *self)
{
    if (self == 0) {
        return;
    }
    self->is_on = 1;
    printf("[Receiver] LED ON\n");
}

void led_receiver_off(led_receiver_t *self)
{
    if (self == 0) {
        return;
    }
    self->is_on = 0;
    printf("[Receiver] LED OFF\n");
}

void motor_receiver_init(motor_receiver_t *self)
{
    if (self == 0) {
        return;
    }
    self->speed = 0;
}

void motor_receiver_set_speed(motor_receiver_t *self, int speed)
{
    if (self == 0) {
        return;
    }
    self->speed = speed;
    printf("[Receiver] MOTOR speed=%d\n", speed);
}

void config_receiver_init(config_receiver_t *self)
{
    if (self == 0) {
        return;
    }
    self->last_saved_config_id = -1;
}

void config_receiver_save(config_receiver_t *self, int config_id)
{
    volatile int i;

    if (self == 0) {
        return;
    }

    printf("[Receiver] save config start: %d (耗时任务开始)\n", config_id);

    /*
     * 用空循环模拟耗时持久化写入。
     * 注意：这里故意放在执行线程中调用，体现通信线程不会被阻塞。
     */
    for (i = 0; i < 5000000; i++) {
        /* busy loop */
    }

    self->last_saved_config_id = config_id;
    printf("[Receiver] save config done : %d (耗时任务完成)\n", config_id);
}
