#include "flash_log_storage.h"

#include <stdio.h>
#include <string.h>

extern log_iterator_t *flash_log_iterator_create(flash_log_storage_t *storage);

void flash_log_init(flash_log_storage_t *self)
{
    if (self == 0) {
        return;
    }

    self->count = 0;
    memset(self->buffer, 0, sizeof(self->buffer));
}

void flash_log_add(flash_log_storage_t *self, const log_entry_t *entry)
{
    if (self == 0 || entry == 0) {
        return;
    }

    if (self->count >= FLASH_LOG_CAPACITY) {
        printf("[FlashStorage] full, ignore log id=%d\n", entry->id);
        return;
    }

    self->buffer[self->count] = *entry;
    self->count++;
}

log_iterator_t *flash_log_create_iterator(flash_log_storage_t *self)
{
    return flash_log_iterator_create(self);
}
