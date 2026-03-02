#pragma once

#include "phy.h"

/* 创建 TCP 传输实现对象 */
phy_t *phy_tcp_create(void);
/* 销毁 TCP 传输实现对象 */
void phy_tcp_destroy(phy_t *phy);
