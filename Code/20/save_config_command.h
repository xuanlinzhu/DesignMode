#ifndef SAVE_CONFIG_COMMAND_H
#define SAVE_CONFIG_COMMAND_H

#include "command.h"
#include "receiver.h"

typedef struct
{
    command_t base;
    config_receiver_t *receiver;
    int config_id;
} save_config_command_t;

void save_config_command_init(save_config_command_t *self, config_receiver_t *receiver, int config_id);

#endif
