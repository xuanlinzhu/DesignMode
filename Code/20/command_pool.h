#ifndef COMMAND_POOL_H
#define COMMAND_POOL_H

#include "command.h"
#include "receiver.h"

#define COMMAND_POOL_CAPACITY 16

/*
 * 命令对象静态池：
 * - 满足“尽量不使用动态分配”的嵌入式约束。
 * - 每个 create 函数从池中借一个槽位；destroy 时归还槽位。
 */
void command_pool_init(void);

command_t *led_on_command_create(led_receiver_t *receiver);
command_t *led_off_command_create(led_receiver_t *receiver);
command_t *motor_set_speed_command_create(motor_receiver_t *receiver, int speed);
command_t *save_config_command_create(config_receiver_t *receiver, int config_id);

/* 内部函数：供各具体命令 destroy 回收对象。 */
void command_pool_release(command_t *cmd);

#endif
