#pragma once

#include <stddef.h>
#include <stdint.h>

#include "phy.h"

/* 协议层抽象对象前置声明 */
typedef struct protocol protocol_t;

/* 协议层能力接口：与具体协议类型解耦 */
typedef struct {
    int (*init)(protocol_t *p);
    int (*tx)(protocol_t *p, const uint8_t *payload, size_t len);
    int (*rx)(protocol_t *p, uint8_t *out, size_t out_len, size_t *out_msg_len);
} protocol_ops_t;

/* Bridge 核心：协议对象持有 phy，实现两个维度独立扩展 */
struct protocol {
    const protocol_ops_t *ops;
    phy_t *phy;
    void *ctx;
};
