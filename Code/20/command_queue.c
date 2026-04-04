#include "command_queue.h"

void command_queue_init(command_queue_t *q)
{
    int i;

    if (q == 0) {
        return;
    }

    q->head = 0;
    q->tail = 0;
    q->count = 0;
    for (i = 0; i < COMMAND_QUEUE_CAPACITY; i++) {
        q->items[i] = 0;
    }
}

int command_queue_push(command_queue_t *q, command_t *cmd)
{
    if (q == 0 || cmd == 0) {
        return 0;
    }

    if (command_queue_is_full(q)) {
        return 0;
    }

    q->items[q->tail] = cmd;
    q->tail = (q->tail + 1) % COMMAND_QUEUE_CAPACITY;
    q->count++;
    return 1;
}

command_t *command_queue_pop(command_queue_t *q)
{
    command_t *cmd;

    if (q == 0 || command_queue_is_empty(q)) {
        return 0;
    }

    cmd = q->items[q->head];
    q->items[q->head] = 0;
    q->head = (q->head + 1) % COMMAND_QUEUE_CAPACITY;
    q->count--;
    return cmd;
}

int command_queue_is_empty(const command_queue_t *q)
{
    if (q == 0) {
        return 1;
    }
    return q->count == 0;
}

int command_queue_is_full(const command_queue_t *q)
{
    if (q == 0) {
        return 0;
    }
    return q->count >= COMMAND_QUEUE_CAPACITY;
}
