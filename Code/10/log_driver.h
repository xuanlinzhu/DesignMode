#ifndef LOG_DRIVER_H
#define LOG_DRIVER_H

#include <stddef.h>
#include <stdint.h>

typedef struct glog_config_t glog_config_t;

typedef struct {
    int level;
    /* Core 已格式化好的最终文本（plain text） */
    const char *text;
    size_t text_len;
    /* 保留原始调用点信息，便于 driver 做诊断或扩展 */
    const char *file;
    int line;
    /* 记录时间戳，便于后续对接 flash/远端上报 */
    uint64_t ts_ms;
} log_record_t;

typedef struct {
    int (*open)(void *ctx, const glog_config_t *core_cfg);
    /* 输出一条日志记录 */
    int (*write)(void *ctx, const log_record_t *rec);
    /* 可选刷盘/刷终端 */
    int (*flush)(void *ctx);
    /* 关闭驱动并释放资源 */
    void (*close)(void *ctx);
} log_driver_ops_t;

typedef struct {
    const char *name;
    const log_driver_ops_t *ops;
    /* driver 私有上下文，由具体实现解释 */
    void *ctx;
    int enabled;
} log_driver_t;

const log_driver_t *log_driver_console_get(void);

#endif
