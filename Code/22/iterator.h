#ifndef ITERATOR_H
#define ITERATOR_H

#include "log_entry.h"

/* 统一日志迭代器接口。 */
typedef struct log_iterator
{
    void (*first)(struct log_iterator *self);
    void (*next)(struct log_iterator *self);
    int (*is_done)(struct log_iterator *self);
    log_entry_t *(*current)(struct log_iterator *self);
} log_iterator_t;

#endif
