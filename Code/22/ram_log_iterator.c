#include "ram_log_storage.h"

#define MAX_RAM_ITERATOR 4

typedef struct
{
    log_iterator_t base;
    int used;
    ram_log_storage_t *storage;
    int index;
    int iterated;
} ram_log_iterator_t;

static ram_log_iterator_t g_pool[MAX_RAM_ITERATOR];

static int ram_oldest_index(const ram_log_storage_t *storage)
{
    int oldest;

    if (storage == 0 || storage->count == 0) {
        return 0;
    }

    oldest = storage->head - storage->count + 1;
    while (oldest < 0) {
        oldest += RAM_LOG_CAPACITY;
    }
    return oldest % RAM_LOG_CAPACITY;
}

static void ram_it_first(log_iterator_t *base)
{
    ram_log_iterator_t *self = (ram_log_iterator_t *)base;

    if (self == 0 || self->storage == 0) {
        return;
    }

    self->iterated = 0;
    self->index = ram_oldest_index(self->storage);
}

static void ram_it_next(log_iterator_t *base)
{
    ram_log_iterator_t *self = (ram_log_iterator_t *)base;

    if (self == 0 || self->storage == 0 || self->iterated >= self->storage->count) {
        return;
    }

    self->iterated++;
    self->index = (self->index + 1) % RAM_LOG_CAPACITY;
}

static int ram_it_is_done(log_iterator_t *base)
{
    ram_log_iterator_t *self = (ram_log_iterator_t *)base;

    if (self == 0 || self->storage == 0) {
        return 1;
    }
    return self->iterated >= self->storage->count;
}

static log_entry_t *ram_it_current(log_iterator_t *base)
{
    ram_log_iterator_t *self = (ram_log_iterator_t *)base;

    if (self == 0 || self->storage == 0 || self->iterated >= self->storage->count) {
        return 0;
    }
    return &self->storage->buffer[self->index];
}

log_iterator_t *ram_log_iterator_create(ram_log_storage_t *storage)
{
    int i;

    for (i = 0; i < MAX_RAM_ITERATOR; i++) {
        if (!g_pool[i].used) {
            g_pool[i].used = 1;
            g_pool[i].storage = storage;
            g_pool[i].base.first = ram_it_first;
            g_pool[i].base.next = ram_it_next;
            g_pool[i].base.is_done = ram_it_is_done;
            g_pool[i].base.current = ram_it_current;
            g_pool[i].index = 0;
            g_pool[i].iterated = 0;
            return &g_pool[i].base;
        }
    }
    return 0;
}
