#pragma once

#include "protocol.h"

/* 创建 Modbus 协议对象 */
protocol_t *modbus_protocol_create(void);
/* 销毁 Modbus 协议对象 */
void modbus_protocol_destroy(protocol_t *p);
