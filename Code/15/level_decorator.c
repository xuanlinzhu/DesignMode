#include "log_decorator.h"

#include <stddef.h>

extern void *malloc(size_t size);
extern void free(void *ptr);

/* ConcreteDecorator: 日志等级装饰器 */
typedef struct {
    LogWriterDecorator base;
    /* 装饰器实例创建后固定一个等级字符串 */
    const char *level_text;
} LevelDecorator;

/* 等级枚举到文本的映射 */
static const char *level_to_text(LogLevel level)
{
    if (level == LOG_LEVEL_INFO) {
        return "INFO";
    }
    if (level == LOG_LEVEL_WARN) {
        return "WARN";
    }
    if (level == LOG_LEVEL_ERROR) {
        return "ERROR";
    }
    return "INFO";
}

static int level_decorator_write(LogWriter *self, const char *message)
{
    LevelDecorator *decorator;
    char merged[512];

    if (self == NULL || message == NULL) {
        return -1;
    }

    decorator = (LevelDecorator *)self;
    if (decorator->base.wrapped == NULL || decorator->base.wrapped->write == NULL) {
        return -1;
    }

    /* 将 [LEVEL] 标签插入消息前缀区 */
    if (log_decorator_insert_tag(decorator->level_text, message, merged, sizeof(merged)) != 0) {
        return -1;
    }

    return decorator->base.wrapped->write(decorator->base.wrapped, merged);
}

static void level_decorator_destroy(LogWriter *self)
{
    LevelDecorator *decorator;

    if (self == NULL) {
        return;
    }

    decorator = (LevelDecorator *)self;
    if (decorator->base.wrapped != NULL && decorator->base.wrapped->destroy != NULL) {
        /* 递归释放下游 writer，防止链上泄漏 */
        decorator->base.wrapped->destroy(decorator->base.wrapped);
        decorator->base.wrapped = NULL;
    }

    free(decorator);
}

LogWriter *level_decorator_create(LogWriter *wrapped, LogLevel level)
{
    LevelDecorator *decorator;

    if (wrapped == NULL || wrapped->write == NULL || wrapped->destroy == NULL) {
        return NULL;
    }

    decorator = (LevelDecorator *)malloc(sizeof(*decorator));
    if (decorator == NULL) {
        return NULL;
    }

    decorator->base.base.write = level_decorator_write;
    decorator->base.base.destroy = level_decorator_destroy;
    decorator->base.wrapped = wrapped;
    /* 创建时决定等级，后续 write 不再变化 */
    decorator->level_text = level_to_text(level);
    return &decorator->base.base;
}
