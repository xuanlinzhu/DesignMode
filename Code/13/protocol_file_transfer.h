#pragma once

#include "protocol.h"

/* 创建文件传输协议对象 */
protocol_t *file_transfer_protocol_create(void);
/* 销毁文件传输协议对象 */
void file_transfer_protocol_destroy(protocol_t *p);
