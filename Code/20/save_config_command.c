#include "save_config_command.h"

#include "command_pool.h"

static void save_config_execute(command_t *base)
{
    save_config_command_t *self = (save_config_command_t *)base;

    if (self == 0 || self->receiver == 0) {
        return;
    }
    config_receiver_save(self->receiver, self->config_id);
}

static void save_config_destroy(command_t *base)
{
    command_pool_release(base);
}

void save_config_command_init(save_config_command_t *self, config_receiver_t *receiver, int config_id)
{
    if (self == 0) {
        return;
    }

    self->base.execute = save_config_execute;
    self->base.destroy = save_config_destroy;
    self->base.name = "SaveConfigCommand";
    self->receiver = receiver;
    self->config_id = config_id;
}
