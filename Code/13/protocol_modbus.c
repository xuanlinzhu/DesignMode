#include "protocol_modbus.h"

#include <stdlib.h>
#include <string.h>

/* Modbus 协议私有状态 */
typedef struct {
    uint8_t slave_addr;
    uint8_t function_code;
} modbus_ctx_t;

static int modbus_init(protocol_t *p)
{
    if (p == NULL || p->phy == NULL || p->phy->ops == NULL) {
        return -1;
    }
    /* 委托到底层传输实现完成链路打开 */
    return p->phy->ops->open(p->phy);
}

static int modbus_tx(protocol_t *p, const uint8_t *payload, size_t len)
{
    modbus_ctx_t *ctx;
    uint8_t frame[260];

    if (p == NULL || p->phy == NULL || p->phy->ops == NULL || payload == NULL || len > 255U) {
        return -1;
    }

    ctx = (modbus_ctx_t *)p->ctx;
    /* 组帧：地址 + 功能码 + 负载长度 + 负载 */
    frame[0] = ctx->slave_addr;
    frame[1] = ctx->function_code;
    frame[2] = (uint8_t)len;
    memcpy(&frame[3], payload, len);

    return p->phy->ops->send(p->phy, frame, len + 3U);
}

static int modbus_rx(protocol_t *p, uint8_t *out, size_t out_len, size_t *out_msg_len)
{
    if (p == NULL || p->phy == NULL || p->phy->ops == NULL) {
        return -1;
    }

    return p->phy->ops->recv(p->phy, out, out_len, out_msg_len);
}

protocol_t *modbus_protocol_create(void)
{
    static const protocol_ops_t ops = {
        modbus_init,
        modbus_tx,
        modbus_rx
    };
    protocol_t *p;
    modbus_ctx_t *ctx;

    p = (protocol_t *)malloc(sizeof(*p));
    ctx = (modbus_ctx_t *)malloc(sizeof(*ctx));
    if (p == NULL || ctx == NULL) {
        free(p);
        free(ctx);
        return NULL;
    }

    ctx->slave_addr = 0x01;
    ctx->function_code = 0x03;

    /* 协议对象只绑定协议行为，具体 phy 由上层在运行期注入 */
    p->ops = &ops;
    p->phy = NULL;
    p->ctx = ctx;
    return p;
}

void modbus_protocol_destroy(protocol_t *p)
{
    if (p == NULL) {
        return;
    }

    free(p->ctx);
    free(p);
}
