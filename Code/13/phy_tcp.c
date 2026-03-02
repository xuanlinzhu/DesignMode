#include "phy_tcp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TCP 具体传输实现的私有上下文 */
typedef struct {
    const char *name;
    const uint8_t *mock_reply;
    size_t mock_reply_len;
    int opened;
} phy_ctx_t;

static int phy_tcp_open(phy_t *phy)
{
    phy_ctx_t *ctx;

    if (phy == NULL || phy->ctx == NULL) {
        return -1;
    }

    ctx = (phy_ctx_t *)phy->ctx;
    /* 模拟打开 TCP 连接 */
    ctx->opened = 1;
    printf("[%s] open\n", ctx->name);
    return 0;
}

static int phy_tcp_close(phy_t *phy)
{
    phy_ctx_t *ctx;

    if (phy == NULL || phy->ctx == NULL) {
        return -1;
    }

    ctx = (phy_ctx_t *)phy->ctx;
    ctx->opened = 0;
    printf("[%s] close\n", ctx->name);
    return 0;
}

static int phy_tcp_send(phy_t *phy, const uint8_t *data, size_t len)
{
    phy_ctx_t *ctx;
    size_t i;

    if (phy == NULL || phy->ctx == NULL || data == NULL) {
        return -1;
    }

    ctx = (phy_ctx_t *)phy->ctx;
    if (ctx->opened == 0) {
        return -1;
    }

    /* 打印发送字节，便于示例观察协议帧内容 */
    printf("[%s] send:", ctx->name);
    for (i = 0; i < len; ++i) {
        printf(" %02X", data[i]);
    }
    printf("\n");
    return 0;
}

static int phy_tcp_recv(phy_t *phy, uint8_t *buf, size_t buf_len, size_t *out_len)
{
    phy_ctx_t *ctx;

    if (phy == NULL || phy->ctx == NULL || buf == NULL || out_len == NULL) {
        return -1;
    }

    ctx = (phy_ctx_t *)phy->ctx;
    if (ctx->opened == 0 || buf_len < ctx->mock_reply_len) {
        return -1;
    }

    /* 复制预置应答，模拟底层收包 */
    memcpy(buf, ctx->mock_reply, ctx->mock_reply_len);
    *out_len = ctx->mock_reply_len;
    printf("[%s] recv: %zu bytes\n", ctx->name, *out_len);
    return 0;
}

phy_t *phy_tcp_create(void)
{
    static const uint8_t reply[] = {0x01, 0x03, 0x02, 0x00, 0x7B};
    static const phy_ops_t ops = {
        phy_tcp_open,
        phy_tcp_close,
        phy_tcp_send,
        phy_tcp_recv
    };
    phy_t *phy;
    phy_ctx_t *ctx;

    /* 组装 TCP 传输对象：行为接口 + 私有上下文 */
    phy = (phy_t *)malloc(sizeof(*phy));
    ctx = (phy_ctx_t *)malloc(sizeof(*ctx));
    if (phy == NULL || ctx == NULL) {
        free(phy);
        free(ctx);
        return NULL;
    }

    ctx->name = "TCP";
    ctx->mock_reply = reply;
    ctx->mock_reply_len = sizeof(reply);
    ctx->opened = 0;

    phy->ops = &ops;
    phy->ctx = ctx;
    return phy;
}

void phy_tcp_destroy(phy_t *phy)
{
    if (phy == NULL) {
        return;
    }

    free(phy->ctx);
    free(phy);
}
