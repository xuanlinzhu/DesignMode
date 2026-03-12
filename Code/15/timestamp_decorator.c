#include "log_decorator.h"

#include <stddef.h>

/*
 * 为减少外部头文件依赖，保留必要的时间类型声明。
 * 仅用于本示例所需字段与函数签名。
 */
typedef long long time_t;

struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

extern int snprintf(char *str, size_t size, const char *format, ...);
extern void *malloc(size_t size);
extern void free(void *ptr);
extern time_t time(time_t *timer);
extern struct tm *localtime(const time_t *timer);
extern size_t strftime(char *s, size_t max, const char *format, const struct tm *tm);

/* ConcreteDecorator: 时间戳装饰器 */
typedef struct {
    LogWriterDecorator base;
} TimestampDecorator;

/*
 * 装饰写入流程：
 * 1) 获取当前本地时间并格式化为字符串
 * 2) 把时间标签插入到消息前缀区域
 * 3) 转发给 wrapped->write 继续下游处理
 */
static int timestamp_decorator_write(LogWriter *self, const char *message)
{
    TimestampDecorator *decorator;
    time_t now;
    const struct tm *tm_ptr;
    struct tm local_tm;
    char time_text[32];
    char merged[512];

    if (self == NULL || message == NULL) {
        return -1;
    }

    decorator = (TimestampDecorator *)self;
    if (decorator->base.wrapped == NULL || decorator->base.wrapped->write == NULL) {
        return -1;
    }

    /* 读取当前时间并转换为本地时间结构 */
    now = time(NULL);
    tm_ptr = localtime(&now);
    if (tm_ptr == NULL) {
        return -1;
    }
    local_tm = *tm_ptr;

    if (strftime(time_text, sizeof(time_text), "%Y-%m-%d %H:%M:%S", &local_tm) == 0U) {
        return -1;
    }

    /* 将 [时间戳] 作为一个标签插入 */
    if (log_decorator_insert_tag(time_text, message, merged, sizeof(merged)) != 0) {
        return -1;
    }

    return decorator->base.wrapped->write(decorator->base.wrapped, merged);
}

static void timestamp_decorator_destroy(LogWriter *self)
{
    TimestampDecorator *decorator;

    if (self == NULL) {
        return;
    }

    decorator = (TimestampDecorator *)self;
    if (decorator->base.wrapped != NULL && decorator->base.wrapped->destroy != NULL) {
        /*
         * 递归销毁：当前装饰器先释放内部对象，
         * 保证从最外层 destroy 一次即可释放整条链。
         */
        decorator->base.wrapped->destroy(decorator->base.wrapped);
        decorator->base.wrapped = NULL;
    }

    free(decorator);
}

LogWriter *timestamp_decorator_create(LogWriter *wrapped)
{
    TimestampDecorator *decorator;

    if (wrapped == NULL || wrapped->write == NULL || wrapped->destroy == NULL) {
        return NULL;
    }

    decorator = (TimestampDecorator *)malloc(sizeof(*decorator));
    if (decorator == NULL) {
        return NULL;
    }

    decorator->base.base.write = timestamp_decorator_write;
    decorator->base.base.destroy = timestamp_decorator_destroy;
    decorator->base.wrapped = wrapped;
    return &decorator->base.base;
}
