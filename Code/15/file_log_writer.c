#include "log_writer.h"

#include <stddef.h>

/*
 * 为了兼容当前 Windows 构建环境与类 POSIX 接口要求，
 * 这里做一层 very-thin 映射：
 * - Windows: _open/_write/_close
 * - 其他平台: open/write/close
 */
#if defined(_WIN32)
#ifndef _O_WRONLY
#define _O_WRONLY 0x0001
#endif
#ifndef _O_APPEND
#define _O_APPEND 0x0008
#endif
#ifndef _O_CREAT
#define _O_CREAT 0x0100
#endif
#ifndef _O_BINARY
#define _O_BINARY 0x8000
#endif
extern int _open(const char *path, int oflag, ...);
extern int _write(int fd, const void *buffer, unsigned int count);
extern int _close(int fd);
#define LOG_OPEN _open
#define LOG_WRITE _write
#define LOG_CLOSE _close
#define LOG_OPEN_FLAGS (_O_CREAT | _O_WRONLY | _O_APPEND | _O_BINARY)
#else
#ifndef O_WRONLY
#define O_WRONLY 01
#endif
#ifndef O_CREAT
#define O_CREAT 0100
#endif
#ifndef O_APPEND
#define O_APPEND 02000
#endif
extern int open(const char *path, int oflag, ...);
extern int write(int fd, const void *buf, unsigned int count);
extern int close(int fd);
#define LOG_OPEN open
#define LOG_WRITE write
#define LOG_CLOSE close
#define LOG_OPEN_FLAGS (O_CREAT | O_WRONLY | O_APPEND)
#endif

extern void *malloc(size_t size);
extern void free(void *ptr);

/*
 * ConcreteComponent：真正落盘的基础写入器。
 * 装饰器链最终都会把 write 调用转发到这里。
 */
typedef struct {
    LogWriter base;
    /* 打开的日志文件描述符 */
    int fd;
} FileLogWriter;

/*
 * 基础写入逻辑：
 * 1) 写完整 message（处理部分写）
 * 2) 追加 '\n' 作为每条日志结束
 */
static int file_log_writer_write(LogWriter *self, const char *message)
{
    FileLogWriter *writer;
    size_t len;
    const char *ptr;
    int n;

    if (self == NULL || message == NULL) {
        return -1;
    }

    writer = (FileLogWriter *)self;
    if (writer->fd < 0) {
        return -1;
    }

    /* 手工计算字符串长度，避免依赖额外库接口 */
    ptr = message;
    len = 0U;
    while (ptr[len] != '\0') {
        len += 1U;
    }

    /* 循环写，直到全部字节写完，处理底层可能的短写 */
    while (len > 0U) {
        n = LOG_WRITE(writer->fd, ptr, (unsigned int)len);
        if (n <= 0) {
            return -1;
        }
        ptr += (size_t)n;
        len -= (size_t)n;
    }

    /* 每条日志统一追加换行，便于日志文件逐行查看 */
    n = LOG_WRITE(writer->fd, "\n", 1U);
    if (n != 1) {
        return -1;
    }

    return 0;
}

static void file_log_writer_destroy(LogWriter *self)
{
    FileLogWriter *writer;

    if (self == NULL) {
        return;
    }

    writer = (FileLogWriter *)self;
    if (writer->fd >= 0) {
        /* 先关闭 fd，再释放对象内存 */
        LOG_CLOSE(writer->fd);
        writer->fd = -1;
    }

    free(writer);
}

/*
 * 创建 FileLogWriter：
 * - 以追加模式打开文件，不覆盖历史日志
 * - 初始化多态函数指针
 */
LogWriter *file_log_writer_create(const char *file_path)
{
    FileLogWriter *writer;

    if (file_path == NULL) {
        return NULL;
    }

    writer = (FileLogWriter *)malloc(sizeof(*writer));
    if (writer == NULL) {
        return NULL;
    }

    writer->fd = LOG_OPEN(file_path, LOG_OPEN_FLAGS, 0644);
    if (writer->fd < 0) {
        /* 打开失败必须回收已分配内存 */
        free(writer);
        return NULL;
    }

    writer->base.write = file_log_writer_write;
    writer->base.destroy = file_log_writer_destroy;
    return &writer->base;
}
