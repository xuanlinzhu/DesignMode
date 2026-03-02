#include "phy_uart.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* UART 具体传输实现的私有上下文 */
typedef struct {
    const char *name;
    const uint8_t *mock_reply;
    size_t mock_reply_len;
    int opened;
} phy_ctx_t;

static int phy_uart_open(phy_t *phy)
{
    phy_ctx_t *ctx;

    if (phy == NULL || phy->ctx == NULL) {
        return -1;
    }

    ctx = (phy_ctx_t *)phy->ctx;
    /* 模拟打开串口链路 */
    ctx->opened = 1;
    printf("[%s] open\n", ctx->name);
    return 0;
}

static int phy_uart_close(phy_t *phy)
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

static int phy_uart_send(phy_t *phy, const uint8_t *data, size_t len)
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

static int phy_uart_recv(phy_t *phy, uint8_t *buf, size_t buf_len, size_t *out_len)
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

phy_t *phy_uart_create(void)
{
    static const uint8_t reply[] = {0x01, 0x03, 0x02, 0x00, 0x2A};
    static const phy_ops_t ops = {
        phy_uart_open,
        phy_uart_close,
        phy_uart_send,
        phy_uart_recv
    };
    phy_t *phy;
    phy_ctx_t *ctx;

    /* 组装 UART 传输对象：行为接口 + 私有上下文 */
    phy = (phy_t *)malloc(sizeof(*phy));
    ctx = (phy_ctx_t *)malloc(sizeof(*ctx));
    if (phy == NULL || ctx == NULL) {
        free(phy);
        free(ctx);
        return NULL;
    }

    ctx->name = "UART";
    ctx->mock_reply = reply;
    ctx->mock_reply_len = sizeof(reply);
    ctx->opened = 0;

    phy->ops = &ops;
    phy->ctx = ctx;
    return phy;
}

void phy_uart_destroy(phy_t *phy)
{
    if (phy == NULL) {
        return;
    }

    free(phy->ctx);
    free(phy);
}
