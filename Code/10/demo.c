#include "glog.h"

#include <pthread.h>
#include <stdio.h>

typedef struct {
    int id;
    int loops;
} thread_arg_t;

/* 线程函数：循环打印，验证并发下日志行完整性 */
static void *worker_thread(void *arg) {
    thread_arg_t *targ = (thread_arg_t *)arg;
    int i;
    for (i = 0; i < targ->loops; ++i) {
        GLOG_PRINT(GLOG_LEVEL_INFO, "worker=%d round=%d", targ->id, i);
    }
    return NULL;
}

static void demo_ex_levels(void) {
    glog_config_t cfg;

    cfg.level_threshold = GLOG_LEVEL_DEBUG;
    cfg.color_enable = 1;

    /* ex=0: 仅用户消息 */
    cfg.ex_message_level = 0;
    (void)glog_configure(&cfg);
    (void)glog_init(NULL);
    GLOG_PRINT(GLOG_LEVEL_INFO, "ex=0: user message only");
    glog_close();

    /* ex=1: 附带时间 */
    cfg.ex_message_level = 1;
    (void)glog_configure(&cfg);
    (void)glog_init(NULL);
    GLOG_PRINT(GLOG_LEVEL_INFO, "ex=1: include [time]");
    glog_close();

    /* ex=2: 附带时间与文件行号 */
    cfg.ex_message_level = 2;
    (void)glog_configure(&cfg);
    (void)glog_init(NULL);
    GLOG_PRINT(GLOG_LEVEL_INFO, "ex=2: include [time] [file:line]");
    glog_close();
}

static void demo_idempotent_init(void) {
    glog_config_t cfg;

    cfg.level_threshold = GLOG_LEVEL_DEBUG;
    cfg.ex_message_level = 2;
    cfg.color_enable = 1;

    /* 重复初始化应保持幂等，不会重复构造核心实例 */
    (void)glog_configure(&cfg);
    (void)glog_init(NULL);
    (void)glog_init(NULL);
    GLOG_PRINT(GLOG_LEVEL_INFO, "init called twice, singleton still one core");
    glog_close();
}

static void demo_record_level(void) {
    glog_config_t cfg;
    int rec;

    cfg.level_threshold = GLOG_LEVEL_DEBUG;
    cfg.ex_message_level = 1;
    cfg.color_enable = 1;

    /* 记录最严重等级，ERR(1) 应覆盖 WARN/DEBUG */
    glog_reset_record_level();
    (void)glog_configure(&cfg);
    (void)glog_init(NULL);
    GLOG_PRINT(GLOG_LEVEL_DEBUG, "debug log");
    GLOG_PRINT(GLOG_LEVEL_WARN, "warn log");
    GLOG_PRINT(GLOG_LEVEL_ERR, "error log");
    rec = glog_get_record_level();
    printf("record_level=%d (expect %d for ERR)\n", rec, GLOG_LEVEL_ERR);
    glog_reset_record_level();
    rec = glog_get_record_level();
    printf("record_level_after_reset=%d (expect %d)\n", rec, GLOG_LEVEL_NONE);
    glog_close();
}

static void demo_multithread(void) {
    glog_config_t cfg;
    pthread_t t1;
    pthread_t t2;
    thread_arg_t a1;
    thread_arg_t a2;

    cfg.level_threshold = GLOG_LEVEL_DEBUG;
    cfg.ex_message_level = 2;
    cfg.color_enable = 1;
    (void)glog_configure(&cfg);
    (void)glog_init(NULL);

    a1.id = 1;
    a1.loops = 5;
    a2.id = 2;
    a2.loops = 5;

    /* 两个线程并发写日志，观察输出是否按行完整 */
    (void)pthread_create(&t1, NULL, worker_thread, &a1);
    (void)pthread_create(&t2, NULL, worker_thread, &a2);
    (void)pthread_join(t1, NULL);
    (void)pthread_join(t2, NULL);

    glog_close();
}

int main(void) {
    printf("Singleton demo: glog core + console driver\n");
    demo_idempotent_init();
    demo_ex_levels();
    demo_record_level();
    demo_multithread();
    return 0;
}
