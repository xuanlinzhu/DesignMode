#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 统一抽象接口（Component）前置声明。
 * 客户端始终通过 LogWriter* 访问对象，避免依赖具体实现。
 */
typedef struct log_writer LogWriter;

/*
 * 多态接口函数指针：
 * - write   : 写一条日志，成功返回 0，失败返回负值
 * - destroy : 释放对象资源（可递归释放整条装饰链）
 */
typedef int (*log_writer_write_fn)(LogWriter *self, const char *message);
typedef void (*log_writer_destroy_fn)(LogWriter *self);

/* Component */
struct log_writer {
    /* 写日志操作 */
    log_writer_write_fn write;
    /* 销毁操作 */
    log_writer_destroy_fn destroy;
};

/*
 * 日志等级定义。
 * LevelDecorator 在创建时固定绑定其中一个等级文本。
 */
typedef enum {
    LOG_LEVEL_INFO = 0,
    LOG_LEVEL_WARN = 1,
    LOG_LEVEL_ERROR = 2
} LogLevel;

/* ConcreteComponent */
/* 创建文件写入器（底层负责 open/write/close） */
LogWriter *file_log_writer_create(const char *file_path);

/* ConcreteDecorator */
/* 时间戳装饰器：在消息前附加 [YYYY-MM-DD HH:MM:SS] */
LogWriter *timestamp_decorator_create(LogWriter *wrapped);
/* 等级装饰器：在消息前附加 [INFO]/[WARN]/[ERROR] */
LogWriter *level_decorator_create(LogWriter *wrapped, LogLevel level);
/* 模块装饰器：在消息前附加 [Module] */
LogWriter *module_decorator_create(LogWriter *wrapped, const char *module_name);

#ifdef __cplusplus
}
#endif
