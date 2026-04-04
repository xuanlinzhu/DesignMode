#ifndef RATELIMIT_HANDLER_H
#define RATELIMIT_HANDLER_H

#include "handler.h"

/*
 * 请求频率限制节点。
 *
 * 当请求频率超过阈值时拒绝，避免连接建立前就进入异常高频请求状态。
 */
typedef struct
{
    handler_t base;
} ratelimit_handler_t;

/* 初始化频率限制节点。 */
void ratelimit_handler_init(ratelimit_handler_t *self);

#endif
