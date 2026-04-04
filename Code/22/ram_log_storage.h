#ifndef RAM_LOG_STORAGE_H
#define RAM_LOG_STORAGE_H

#include "iterator.h"

#define RAM_LOG_CAPACITY 8

typedef struct
{
    log_entry_t buffer[RAM_LOG_CAPACITY];
    int head;
    int count;
} ram_log_storage_t;

void ram_log_init(ram_log_storage_t *self);
void ram_log_add(ram_log_storage_t *self, const log_entry_t *entry);
log_iterator_t *ram_log_create_iterator(ram_log_storage_t *self);

#endif
