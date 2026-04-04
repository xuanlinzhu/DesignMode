#ifndef BLACKLIST_HANDLER_H
#define BLACKLIST_HANDLER_H

#include "handler.h"

/*
 * 黑名单检查节点。
 *
 * 规则：
 * - ip == 0 视为命中黑名单，直接拒绝
 * - 其他情况继续传递给后继节点
 */
typedef struct
{
    handler_t base;
} blacklist_handler_t;

/* 初始化黑名单检查节点。 */
void blacklist_handler_init(blacklist_handler_t *self);

#endif
