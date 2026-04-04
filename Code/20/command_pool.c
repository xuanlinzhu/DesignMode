#include "command_pool.h"

#include "led_off_command.h"
#include "led_on_command.h"
#include "motor_set_speed_command.h"
#include "save_config_command.h"

typedef union
{
    led_on_command_t led_on;
    led_off_command_t led_off;
    motor_set_speed_command_t motor_set;
    save_config_command_t save_cfg;
} command_storage_t;

typedef struct
{
    int used;
    command_storage_t storage;
} command_slot_t;

static command_slot_t g_pool[COMMAND_POOL_CAPACITY];

static command_slot_t *alloc_slot(void)
{
    int i;

    for (i = 0; i < COMMAND_POOL_CAPACITY; i++) {
        if (!g_pool[i].used) {
            g_pool[i].used = 1;
            return &g_pool[i];
        }
    }
    return 0;
}

void command_pool_init(void)
{
    int i;

    for (i = 0; i < COMMAND_POOL_CAPACITY; i++) {
        g_pool[i].used = 0;
    }
}

void command_pool_release(command_t *cmd)
{
    int i;

    if (cmd == 0) {
        return;
    }

    for (i = 0; i < COMMAND_POOL_CAPACITY; i++) {
        command_t *base = (command_t *)&g_pool[i].storage;
        if (base == cmd) {
            g_pool[i].used = 0;
            return;
        }
    }
}

command_t *led_on_command_create(led_receiver_t *receiver)
{
    command_slot_t *slot = alloc_slot();

    if (slot == 0) {
        return 0;
    }
    led_on_command_init(&slot->storage.led_on, receiver);
    return &slot->storage.led_on.base;
}

command_t *led_off_command_create(led_receiver_t *receiver)
{
    command_slot_t *slot = alloc_slot();

    if (slot == 0) {
        return 0;
    }
    led_off_command_init(&slot->storage.led_off, receiver);
    return &slot->storage.led_off.base;
}

command_t *motor_set_speed_command_create(motor_receiver_t *receiver, int speed)
{
    command_slot_t *slot = alloc_slot();

    if (slot == 0) {
        return 0;
    }
    motor_set_speed_command_init(&slot->storage.motor_set, receiver, speed);
    return &slot->storage.motor_set.base;
}

command_t *save_config_command_create(config_receiver_t *receiver, int config_id)
{
    command_slot_t *slot = alloc_slot();

    if (slot == 0) {
        return 0;
    }
    save_config_command_init(&slot->storage.save_cfg, receiver, config_id);
    return &slot->storage.save_cfg.base;
}
