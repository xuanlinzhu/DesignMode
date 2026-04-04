#include "flash_log_storage.h"

#define MAX_FLASH_ITERATOR 4

typedef struct
{
    log_iterator_t base;
    int used;
    flash_log_storage_t *storage;
    int index;
} flash_log_iterator_t;

static flash_log_iterator_t g_pool[MAX_FLASH_ITERATOR];

static void flash_it_first(log_iterator_t *base)
{
    flash_log_iterator_t *self = (flash_log_iterator_t *)base;

    if (self == 0) {
        return;
    }
    self->index = 0;
}

static void flash_it_next(log_iterator_t *base)
{
    flash_log_iterator_t *self = (flash_log_iterator_t *)base;

    if (self == 0 || self->storage == 0 || self->index >= self->storage->count) {
        return;
    }
    self->index++;
}

static int flash_it_is_done(log_iterator_t *base)
{
    flash_log_iterator_t *self = (flash_log_iterator_t *)base;

    if (self == 0 || self->storage == 0) {
        return 1;
    }
    return self->index >= self->storage->count;
}

static log_entry_t *flash_it_current(log_iterator_t *base)
{
    flash_log_iterator_t *self = (flash_log_iterator_t *)base;

    if (self == 0 || self->storage == 0 || self->index >= self->storage->count) {
        return 0;
    }
    return &self->storage->buffer[self->index];
}

log_iterator_t *flash_log_iterator_create(flash_log_storage_t *storage)
{
    int i;

    for (i = 0; i < MAX_FLASH_ITERATOR; i++) {
        if (!g_pool[i].used) {
            g_pool[i].used = 1;
            g_pool[i].storage = storage;
            g_pool[i].base.first = flash_it_first;
            g_pool[i].base.next = flash_it_next;
            g_pool[i].base.is_done = flash_it_is_done;
            g_pool[i].base.current = flash_it_current;
            g_pool[i].index = 0;
            return &g_pool[i].base;
        }
    }
    return 0;
}
