#ifndef GLOG_H
#define GLOG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct glog_config_t {
    /* 超过阈值的日志会被过滤（数值越小级别越高） */
    int level_threshold;
    /* 扩展信息级别：0=仅消息，1=[时间]，2=[时间][文件:行] */
    int ex_message_level;
    /* 是否启用终端颜色（由 console driver 决定是否生效） */
    int color_enable;
} glog_config_t;

enum {
    GLOG_LEVEL_FATAL = 0,
    GLOG_LEVEL_ERR = 1,
    GLOG_LEVEL_WARN = 2,
    GLOG_LEVEL_INFO = 3,
    GLOG_LEVEL_DEBUG = 4,
    GLOG_LEVEL_NONE = 0x7fffffff
};

int glog_init(const glog_config_t *cfg);
void glog_close(void);
int glog_configure(const glog_config_t *cfg);
void glog_print(int level, const char *file, int line, const char *fmt, ...);
int glog_get_record_level(void);
void glog_reset_record_level(void);

/* 唯一日志入口宏：统一带上调用点文件与行号 */
#define GLOG_PRINT(level, fmt, ...) \
    glog_print((level), __FILE__, __LINE__, (fmt), ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif
