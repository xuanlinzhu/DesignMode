#include "glog.h"
#include "log_driver.h"

#include <pthread.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
    int inited;
    glog_config_t cfg;
    int record_level;
    const log_driver_t *driver;
    pthread_mutex_t lock;
} glog_core_t;

/* 单例控制块：仅在本文件可见 */
static glog_core_t g_core = {
    0,
    {GLOG_LEVEL_INFO, 1, 1},
    GLOG_LEVEL_NONE,
    NULL,
    PTHREAD_MUTEX_INITIALIZER
};

static uint64_t glog_now_ms(void) {
    struct timespec ts;
    (void)timespec_get(&ts, TIME_UTC);
    return (uint64_t)ts.tv_sec * 1000ULL + (uint64_t)ts.tv_nsec / 1000000ULL;
}

static void glog_time_string(char *buf, size_t buf_len) {
    time_t sec;
    struct tm tm_buf;

    if (buf == NULL || buf_len == 0U) {
        return;
    }

    sec = (time_t)(glog_now_ms() / 1000ULL);
#if defined(_WIN32)
    if (localtime_s(&tm_buf, &sec) != 0) {
        (void)snprintf(buf, buf_len, "1970-01-01 00:00:00");
        return;
    }
#else
    if (localtime_r(&sec, &tm_buf) == NULL) {
        (void)snprintf(buf, buf_len, "1970-01-01 00:00:00");
        return;
    }
#endif

    (void)strftime(buf, buf_len, "%Y-%m-%d %H:%M:%S", &tm_buf);
}

static void glog_update_record_level(int level) {
    if (level < g_core.record_level) {
        g_core.record_level = level;
    }
}

static void glog_config_set_defaults(glog_config_t *cfg) {
    if (cfg == NULL) {
        return;
    }
    cfg->level_threshold = GLOG_LEVEL_INFO;
    cfg->ex_message_level = 1;
    cfg->color_enable = 1;
}

static void glog_config_apply(glog_config_t *dst, const glog_config_t *src) {
    if (dst == NULL) {
        return;
    }
    glog_config_set_defaults(dst);
    if (src == NULL) {
        return;
    }
    dst->level_threshold = src->level_threshold;
    if (src->ex_message_level < 0) {
        dst->ex_message_level = 0;
    } else if (src->ex_message_level > 2) {
        dst->ex_message_level = 2;
    } else {
        dst->ex_message_level = src->ex_message_level;
    }
    dst->color_enable = src->color_enable ? 1 : 0;
}

int glog_configure(const glog_config_t *cfg) {
    if (cfg == NULL) {
        return -1;
    }

    (void)pthread_mutex_lock(&g_core.lock);
    glog_config_apply(&g_core.cfg, cfg);
    (void)pthread_mutex_unlock(&g_core.lock);
    return 0;
}

int glog_init(const glog_config_t *cfg) {
    int rc;
    const log_driver_t *driver;

    (void)pthread_mutex_lock(&g_core.lock);

    if (cfg != NULL) {
        glog_config_apply(&g_core.cfg, cfg);
    }

    if (g_core.inited) {
        (void)pthread_mutex_unlock(&g_core.lock);
        return 0;
    }

    driver = log_driver_console_get();
    if (driver == NULL || driver->ops == NULL || driver->ops->open == NULL ||
        driver->ops->write == NULL || driver->ops->close == NULL) {
        (void)pthread_mutex_unlock(&g_core.lock);
        return -1;
    }

    rc = driver->ops->open(driver->ctx, &g_core.cfg);
    if (rc != 0) {
        (void)pthread_mutex_unlock(&g_core.lock);
        return -1;
    }

    g_core.driver = driver;
    g_core.inited = 1;
    (void)pthread_mutex_unlock(&g_core.lock);
    return 0;
}

void glog_close(void) {
    (void)pthread_mutex_lock(&g_core.lock);
    if (g_core.inited && g_core.driver != NULL && g_core.driver->ops != NULL &&
        g_core.driver->ops->close != NULL) {
        g_core.driver->ops->close(g_core.driver->ctx);
    }
    g_core.driver = NULL;
    g_core.inited = 0;
    (void)pthread_mutex_unlock(&g_core.lock);
}

void glog_print(int level, const char *file, int line, const char *fmt, ...) {
    char user_msg[768];
    char time_buf[32];
    char final_text[1024];
    int wrote;
    va_list ap;
    log_record_t rec;

    if (fmt == NULL) {
        return;
    }

    (void)pthread_mutex_lock(&g_core.lock);
    glog_update_record_level(level);

    if (!g_core.inited) {
        (void)pthread_mutex_unlock(&g_core.lock);
        return;
    }

    if (level > g_core.cfg.level_threshold) {
        (void)pthread_mutex_unlock(&g_core.lock);
        return;
    }

    va_start(ap, fmt);
    wrote = vsnprintf(user_msg, sizeof(user_msg), fmt, ap);
    va_end(ap);
    if (wrote < 0) {
        (void)pthread_mutex_unlock(&g_core.lock);
        return;
    }

    if (g_core.cfg.ex_message_level == 0) {
        (void)snprintf(final_text, sizeof(final_text), "%s", user_msg);
    } else {
        glog_time_string(time_buf, sizeof(time_buf));
        if (g_core.cfg.ex_message_level == 1) {
            (void)snprintf(final_text, sizeof(final_text), "[%s] %s", time_buf, user_msg);
        } else {
            const char *src = (file != NULL) ? file : "unknown";
            (void)snprintf(final_text, sizeof(final_text), "[%s] [%s:%d] %s", time_buf, src, line,
                           user_msg);
        }
    }

    rec.level = level;
    rec.text = final_text;
    rec.text_len = strlen(final_text);
    rec.file = file;
    rec.line = line;
    rec.ts_ms = glog_now_ms();

    (void)g_core.driver->ops->write(g_core.driver->ctx, &rec);
    if (g_core.driver->ops->flush != NULL) {
        (void)g_core.driver->ops->flush(g_core.driver->ctx);
    }

    (void)pthread_mutex_unlock(&g_core.lock);
}

int glog_get_record_level(void) {
    int level;
    (void)pthread_mutex_lock(&g_core.lock);
    level = g_core.record_level;
    (void)pthread_mutex_unlock(&g_core.lock);
    return level;
}

void glog_reset_record_level(void) {
    (void)pthread_mutex_lock(&g_core.lock);
    g_core.record_level = GLOG_LEVEL_NONE;
    (void)pthread_mutex_unlock(&g_core.lock);
}
