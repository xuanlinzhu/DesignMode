#include "protocol_file_transfer.h"

#include <stdlib.h>
#include <string.h>

/* 文件传输协议私有状态 */
typedef struct {
    const char *file_name;
} file_transfer_ctx_t;

static int file_transfer_init(protocol_t *p)
{
    if (p == NULL || p->phy == NULL || p->phy->ops == NULL) {
        return -1;
    }
    /* 委托到底层传输实现完成链路打开 */
    return p->phy->ops->open(p->phy);
}

static int file_transfer_tx(protocol_t *p, const uint8_t *payload, size_t len)
{
    file_transfer_ctx_t *ctx;
    uint8_t frame[260];
    size_t name_len;

    if (p == NULL || p->phy == NULL || p->phy->ops == NULL || payload == NULL) {
        return -1;
    }

    ctx = (file_transfer_ctx_t *)p->ctx;
    name_len = strlen(ctx->file_name);
    if (name_len > 31U || len > 220U) {
        return -1;
    }

    /* 组帧：协议标识 + 文件名长度 + 文件名 + 数据长度 + 数据 */
    frame[0] = 0xA1;
    frame[1] = (uint8_t)name_len;
    memcpy(&frame[2], ctx->file_name, name_len);
    frame[2 + name_len] = (uint8_t)len;
    memcpy(&frame[3 + name_len], payload, len);

    return p->phy->ops->send(p->phy, frame, 3U + name_len + len);
}

static int file_transfer_rx(protocol_t *p, uint8_t *out, size_t out_len, size_t *out_msg_len)
{
    if (p == NULL || p->phy == NULL || p->phy->ops == NULL) {
        return -1;
    }

    return p->phy->ops->recv(p->phy, out, out_len, out_msg_len);
}

protocol_t *file_transfer_protocol_create(void)
{
    static const protocol_ops_t ops = {
        file_transfer_init,
        file_transfer_tx,
        file_transfer_rx
    };
    protocol_t *p;
    file_transfer_ctx_t *ctx;

    p = (protocol_t *)malloc(sizeof(*p));
    ctx = (file_transfer_ctx_t *)malloc(sizeof(*ctx));
    if (p == NULL || ctx == NULL) {
        free(p);
        free(ctx);
        return NULL;
    }

    ctx->file_name = "firmware.bin";

    p->ops = &ops;
    p->phy = NULL;
    p->ctx = ctx;
    return p;
}

void file_transfer_protocol_destroy(protocol_t *p)
{
    if (p == NULL) {
        return;
    }

    free(p->ctx);
    free(p);
}
