#include "ram_log_storage.h"

#include <string.h>

extern log_iterator_t *ram_log_iterator_create(ram_log_storage_t *storage);

void ram_log_init(ram_log_storage_t *self)
{
    if (self == 0) {
        return;
    }

    self->head = -1;
    self->count = 0;
    memset(self->buffer, 0, sizeof(self->buffer));
}

void ram_log_add(ram_log_storage_t *self, const log_entry_t *entry)
{
    int index;

    if (self == 0 || entry == 0) {
        return;
    }

    if (self->count == 0) {
        self->head = 0;
    } else {
        self->head = (self->head + 1) % RAM_LOG_CAPACITY;
    }

    index = self->head;
    self->buffer[index] = *entry;

    if (self->count < RAM_LOG_CAPACITY) {
        self->count++;
    }
}

log_iterator_t *ram_log_create_iterator(ram_log_storage_t *self)
{
    return ram_log_iterator_create(self);
}
