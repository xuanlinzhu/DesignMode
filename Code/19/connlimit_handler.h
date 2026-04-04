#ifndef CONNLIMIT_HANDLER_H
#define CONNLIMIT_HANDLER_H

#include "handler.h"

/*
 * 连接数限制节点。
 *
 * 当前连接数达到或超过上限时，直接拒绝当前连接请求。
 */
typedef struct
{
    handler_t base;
} connlimit_handler_t;

/* 初始化连接数限制节点。 */
void connlimit_handler_init(connlimit_handler_t *self);

#endif
