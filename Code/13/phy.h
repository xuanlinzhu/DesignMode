#pragma once

#include <stddef.h>
#include <stdint.h>

/* 传输层抽象对象前置声明 */
typedef struct phy phy_t;

/* 传输层能力接口：协议层只通过这组函数访问底层 */
typedef struct {
    int (*open)(phy_t *phy);
    int (*close)(phy_t *phy);
    int (*send)(phy_t *phy, const uint8_t *data, size_t len);
    int (*recv)(phy_t *phy, uint8_t *buf, size_t buf_len, size_t *out_len);
} phy_ops_t;

/* 统一传输对象：ops 描述行为，ctx 存放具体实现私有状态 */
struct phy {
    const phy_ops_t *ops;
    void *ctx;
};
