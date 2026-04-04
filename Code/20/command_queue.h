#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include "command.h"

#define COMMAND_QUEUE_CAPACITY 16

/*
 * 命令队列（Invoker）：
 * - 使用固定容量循环队列，避免动态内存。
 * - 存储的只是 command_t*，不关心具体命令细节。
 */
typedef struct
{
    command_t *items[COMMAND_QUEUE_CAPACITY];
    int head;
    int tail;
    int count;
} command_queue_t;

void command_queue_init(command_queue_t *q);
int command_queue_push(command_queue_t *q, command_t *cmd);
command_t *command_queue_pop(command_queue_t *q);
int command_queue_is_empty(const command_queue_t *q);
int command_queue_is_full(const command_queue_t *q);

#endif
