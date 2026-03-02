#include "glog.h"
#include "log_driver.h"

#include <stdio.h>

#if defined(_WIN32)
#include <windows.h>
#include <io.h>
#define GLOG_ISATTY _isatty
#define GLOG_FILENO _fileno
#else
#include <unistd.h>
#define GLOG_ISATTY isatty
#define GLOG_FILENO fileno
#endif

typedef struct {
    int color_enable;
#if defined(_WIN32)
    int vt_stdout_enabled;
    int vt_stderr_enabled;
#endif
} console_ctx_t;

#if defined(_WIN32)
static int enable_vt_mode(HANDLE h) {
    DWORD mode;

    if (h == NULL || h == INVALID_HANDLE_VALUE) {
        return 0;
    }
    if (GetConsoleMode(h, &mode) == 0) {
        return 0;
    }
    if ((mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) == 0U) {
        DWORD new_mode = mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (SetConsoleMode(h, new_mode) == 0) {
            return 0;
        }
    }
    return 1;
}
#endif

/* 控制台颜色映射（ANSI） */
static const char *level_color(int level) {
    switch (level) {
        case GLOG_LEVEL_FATAL:
        case GLOG_LEVEL_ERR:
            return "\x1b[31m";
        case GLOG_LEVEL_WARN:
            return "\x1b[33m";
        case GLOG_LEVEL_INFO:
            return "\x1b[32m";
        case GLOG_LEVEL_DEBUG:
            return "\x1b[36m";
        default:
            return "\x1b[0m";
    }
}

static int console_open(void *ctx, const glog_config_t *core_cfg) {
    console_ctx_t *cctx = (console_ctx_t *)ctx;
    if (cctx == NULL || core_cfg == NULL) {
        return -1;
    }
    cctx->color_enable = core_cfg->color_enable ? 1 : 0;
#if defined(_WIN32)
    cctx->vt_stdout_enabled = enable_vt_mode(GetStdHandle(STD_OUTPUT_HANDLE));
    cctx->vt_stderr_enabled = enable_vt_mode(GetStdHandle(STD_ERROR_HANDLE));
#endif
    return 0;
}

static int console_write(void *ctx, const log_record_t *rec) {
    console_ctx_t *cctx = (console_ctx_t *)ctx;
    FILE *stream;
    int is_tty;
    int can_color;

    if (cctx == NULL || rec == NULL || rec->text == NULL) {
        return -1;
    }

    /* 错误级别走 stderr，其余走 stdout */
    stream = (rec->level <= GLOG_LEVEL_ERR) ? stderr : stdout;
    is_tty = GLOG_ISATTY(GLOG_FILENO(stream));

    can_color = cctx->color_enable && is_tty;
#if defined(_WIN32)
    if (cctx->color_enable) {
        can_color = (stream == stderr) ? cctx->vt_stderr_enabled : cctx->vt_stdout_enabled;
    }
#endif

    if (can_color) {
        const char *color = level_color(rec->level);
        (void)fprintf(stream, "%s", color);
        (void)fwrite(rec->text, 1U, rec->text_len, stream);
        (void)fprintf(stream, "\x1b[0m");
    } else {
        (void)fwrite(rec->text, 1U, rec->text_len, stream);
    }
    (void)fputc('\n', stream);

    return 0;
}

static int console_flush(void *ctx) {
    (void)ctx;
    (void)fflush(stdout);
    (void)fflush(stderr);
    return 0;
}

static void console_close(void *ctx) {
    (void)ctx;
}

static console_ctx_t g_console_ctx;

static const log_driver_ops_t g_console_ops = {
    console_open,
    console_write,
    console_flush,
    console_close
};

static const log_driver_t g_console_driver = {
    "console",
    &g_console_ops,
    &g_console_ctx,
    1
};

const log_driver_t *log_driver_console_get(void) {
    return &g_console_driver;
}
