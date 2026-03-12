#pragma once

#include <stddef.h>

#include "log_writer.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 抽象装饰器（Decorator）：
 * 1) base 让它本身也满足 LogWriter 接口（is-a）
 * 2) wrapped 持有被包装对象（has-a）
 * 这是 Decorator 模式最关键的结构特征。
 */
typedef struct {
    LogWriter base;
    LogWriter *wrapped;
} LogWriterDecorator;

/* Decorator: 自身是 LogWriter，同时持有一个 LogWriter* wrapped。 */

/*
 * 在消息前插入一个方括号标签：
 * 输入 message 若已有前缀标签（例如 [TIME] [LEVEL] ...），
 * 会把新标签插入到“已有标签区段”之后，从而保证不同装饰器
 * 按链路顺序叠加但输出顺序稳定、易读。
 */
int log_decorator_insert_tag(const char *tag, const char *message, char *out, size_t out_cap);

#ifdef __cplusplus
}
#endif
