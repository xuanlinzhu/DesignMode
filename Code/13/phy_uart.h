#pragma once

#include "phy.h"

/* 创建 UART 传输实现对象 */
phy_t *phy_uart_create(void);
/* 销毁 UART 传输实现对象 */
void phy_uart_destroy(phy_t *phy);
