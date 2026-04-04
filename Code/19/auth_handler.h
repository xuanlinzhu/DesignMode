#ifndef AUTH_HANDLER_H
#define AUTH_HANDLER_H

#include "handler.h"

/*
 * 认证检查节点。
 *
 * 该节点只负责验证 token 是否有效；若通过，则继续交给链尾结束逻辑，
 * 由 run_chain 在遍历完成后统一给出 ACCEPT。
 */
typedef struct
{
    handler_t base;
} auth_handler_t;

/* 初始化认证检查节点。 */
void auth_handler_init(auth_handler_t *self);

#endif
