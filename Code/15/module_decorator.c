#include "log_decorator.h"

#include <stddef.h>

extern int snprintf(char *str, size_t size, const char *format, ...);
extern void *malloc(size_t size);
extern void free(void *ptr);

/* ConcreteDecorator: 模块名装饰器 */
typedef struct {
    LogWriterDecorator base;
    /* 保存模块名副本，避免依赖外部传入字符串生命周期 */
    char module_name[64];
} ModuleDecorator;

static int module_decorator_write(LogWriter *self, const char *message)
{
    ModuleDecorator *decorator;
    char merged[512];

    if (self == NULL || message == NULL) {
        return -1;
    }

    decorator = (ModuleDecorator *)self;
    if (decorator->base.wrapped == NULL || decorator->base.wrapped->write == NULL) {
        return -1;
    }

    /* 将 [模块名] 标签插入消息前缀区 */
    if (log_decorator_insert_tag(decorator->module_name, message, merged, sizeof(merged)) != 0) {
        return -1;
    }

    return decorator->base.wrapped->write(decorator->base.wrapped, merged);
}

static void module_decorator_destroy(LogWriter *self)
{
    ModuleDecorator *decorator;

    if (self == NULL) {
        return;
    }

    decorator = (ModuleDecorator *)self;
    if (decorator->base.wrapped != NULL && decorator->base.wrapped->destroy != NULL) {
        /* 递归销毁下游对象 */
        decorator->base.wrapped->destroy(decorator->base.wrapped);
        decorator->base.wrapped = NULL;
    }

    free(decorator);
}

LogWriter *module_decorator_create(LogWriter *wrapped, const char *module_name)
{
    ModuleDecorator *decorator;
    int n;

    if (wrapped == NULL || wrapped->write == NULL || wrapped->destroy == NULL || module_name == NULL) {
        return NULL;
    }

    decorator = (ModuleDecorator *)malloc(sizeof(*decorator));
    if (decorator == NULL) {
        return NULL;
    }

    /* 拷贝模块名到内部缓冲，确保装饰器自持有状态 */
    n = snprintf(decorator->module_name, sizeof(decorator->module_name), "%s", module_name);
    if (n < 0 || (size_t)n >= sizeof(decorator->module_name)) {
        free(decorator);
        return NULL;
    }

    decorator->base.base.write = module_decorator_write;
    decorator->base.base.destroy = module_decorator_destroy;
    decorator->base.wrapped = wrapped;
    return &decorator->base.base;
}
