#ifndef RECEIVER_H
#define RECEIVER_H

/*
 * Receiver 角色：
 * 真正执行硬件/业务动作的模块。
 * 这些模块由命令对象调用，通信侧禁止直接调用这些接口。
 */

typedef struct
{
    int is_on;
} led_receiver_t;

typedef struct
{
    int speed;
} motor_receiver_t;

typedef struct
{
    int last_saved_config_id;
} config_receiver_t;

void led_receiver_init(led_receiver_t *self);
void led_receiver_on(led_receiver_t *self);
void led_receiver_off(led_receiver_t *self);

void motor_receiver_init(motor_receiver_t *self);
void motor_receiver_set_speed(motor_receiver_t *self, int speed);

void config_receiver_init(config_receiver_t *self);
void config_receiver_save(config_receiver_t *self, int config_id);

#endif
