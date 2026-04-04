#include "auth_handler.h"
#include "blacklist_handler.h"
#include "connlimit_handler.h"
#include "ratelimit_handler.h"

extern int printf(const char *format, ...);

/*
 * 将责任链执行结果转换成可读文本，便于演示最终结论。
 */
static const char *chain_result_to_string(chain_result_t result)
{
    switch (result)
    {
        case CHAIN_CONTINUE:
            return "CHAIN_CONTINUE";
        case CHAIN_REJECT:
            return "CHAIN_REJECT";
        case CHAIN_ACCEPT:
            return "CHAIN_ACCEPT";
        default:
            return "UNKNOWN";
    }
}

/*
 * 演示单个连接请求进入责任链的完整过程。
 *
 * title 用来标识场景，conn 为当前测试输入。
 * 输出中会先打印场景名，再打印各节点日志，最后打印总结果。
 */
static void run_demo_case(const char *title, handler_t *head, conn_t *conn)
{
    chain_result_t result;

    if (title == 0 || head == 0 || conn == 0) {
        return;
    }

    printf("==============================\n");
    printf("%s\n", title);
    result = run_chain(head, conn);
    printf("[Result] %s\n", chain_result_to_string(result));
}

int main(void)
{
    blacklist_handler_t blacklist;
    connlimit_handler_t connlimit;
    ratelimit_handler_t ratelimit;
    auth_handler_t auth;

    conn_t allow_conn;
    conn_t reject_in_middle;
    conn_t reject_at_head;

    /*
     * 构造固定责任链：
     * blacklist -> conn_limit -> rate_limit -> auth
     *
     * 客户端只持有链头，不直接调用任何具体节点。
     */
    blacklist_handler_init(&blacklist);
    connlimit_handler_init(&connlimit);
    ratelimit_handler_init(&ratelimit);
    auth_handler_init(&auth);

    handler_set_next(&blacklist.base, &connlimit.base);
    handler_set_next(&connlimit.base, &ratelimit.base);
    handler_set_next(&ratelimit.base, &auth.base);

    /* 场景 1：所有节点都放行，最终由 run_chain 返回 ACCEPT。 */
    allow_conn.ip = 12345;
    allow_conn.current_conn = 3;
    allow_conn.max_conn = 10;
    allow_conn.request_freq = 40;
    allow_conn.token_valid = 1;

    /*
     * 场景 2：中间节点连接数限制拒绝。
     * 预期只会看到 Blacklist 和 ConnLimit 两条日志，后续节点不会执行。
     */
    reject_in_middle.ip = 22222;
    reject_in_middle.current_conn = 10;
    reject_in_middle.max_conn = 10;
    reject_in_middle.request_freq = 20;
    reject_in_middle.token_valid = 1;

    /* 场景 3：第一个节点黑名单直接拒绝。 */
    reject_at_head.ip = 0;
    reject_at_head.current_conn = 1;
    reject_at_head.max_conn = 10;
    reject_at_head.request_freq = 10;
    reject_at_head.token_valid = 1;

    run_demo_case("[场景1] 正常通过：所有节点继续传递，最终 ACCEPT", &blacklist.base, &allow_conn);
    run_demo_case("[场景2] 中间节点拒绝：ConnLimit 拒绝，后续节点不执行", &blacklist.base, &reject_in_middle);
    run_demo_case("[场景3] 第一个节点拒绝：Blacklist 直接拒绝", &blacklist.base, &reject_at_head);

    printf("==============================\n");
    return 0;
}
