#ifndef HANDLER_H
#define HANDLER_H

#include <stddef.h>

/*
 * 责任链统一返回值。
 *
 * 所有节点都必须使用同一套语义，避免不同节点对“继续/拒绝/放行”
 * 的理解不一致，导致链式行为难以维护。
 */
typedef enum
{
    CHAIN_CONTINUE = 0,
    CHAIN_REJECT,
    CHAIN_ACCEPT
} chain_result_t;

/*
 * 连接请求对象。
 *
 * 这个结构体会沿着责任链逐级传递，每个节点只读取自己关心的字段，
 * 不需要知道其他节点的内部判断细节。
 */
typedef struct
{
    int ip;
    int current_conn;
    int max_conn;
    int request_freq;
    int token_valid;
} conn_t;

/* 前置声明，便于在函数指针中引用自身类型。 */
struct handler;

/*
 * 责任链节点基类。
 *
 * 约束：
 * 1. handle 负责执行当前节点的独立检查
 * 2. next 指向后继节点
 * 3. 节点之间不能直接互相调用，只能由统一的 run_chain 驱动传递
 */
typedef struct handler
{
    chain_result_t (*handle)(struct handler *self, conn_t *conn);
    struct handler *next;
} handler_t;

/* 设置当前节点的后继节点。 */
void handler_set_next(handler_t *self, handler_t *next);

/*
 * 执行责任链。
 *
 * 行为必须满足设计说明：
 * - 从 head 开始依次调用各节点
 * - 节点返回 CONTINUE 时继续向后传递
 * - 节点返回 REJECT 时立即终止
 * - 若整条链都未拒绝，则最终返回 ACCEPT
 */
chain_result_t run_chain(handler_t *head, conn_t *conn);

#endif
