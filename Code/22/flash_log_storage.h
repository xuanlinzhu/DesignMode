#ifndef FLASH_LOG_STORAGE_H
#define FLASH_LOG_STORAGE_H

#include "iterator.h"

#define FLASH_LOG_CAPACITY 16

typedef struct
{
    log_entry_t buffer[FLASH_LOG_CAPACITY];
    int count;
} flash_log_storage_t;

void flash_log_init(flash_log_storage_t *self);
void flash_log_add(flash_log_storage_t *self, const log_entry_t *entry);
log_iterator_t *flash_log_create_iterator(flash_log_storage_t *self);

#endif
