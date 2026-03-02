#include "freertos_api.h"

#include <pthread.h>
#include <stdlib.h>

/* 适配器内部包装：把 FreeRTOS 任务函数与参数打包传给 pthread 入口 */
typedef struct {
    TaskFunction_t task;
    void *arg;
} task_wrapper_t;

/* pthread 标准入口：解包后调用原任务函数 */
static void *task_entry(void *ctx)
{
    task_wrapper_t *wrapper = (task_wrapper_t *)ctx;
    (void)pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    (void)pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    if (wrapper == NULL || wrapper->task == NULL) {
        free(wrapper);
        return NULL;
    }

    wrapper->task(wrapper->arg);
    free(wrapper);
    return NULL;
}

BaseType_t xTaskCreate(TaskFunction_t task,
                       const char *name,
                       uint16_t stackDepth,
                       void *params,
                       uint32_t priority,
                       TaskHandle_t *handle)
{
    pthread_t thread;
    pthread_attr_t attr;
    task_wrapper_t *wrapper = NULL;
    int ret;

    if (task == NULL) {
        return pdFAIL;
    }

    wrapper = (task_wrapper_t *)malloc(sizeof(task_wrapper_t));
    if (wrapper == NULL) {
        return pdFAIL;
    }

    wrapper->task = task;
    wrapper->arg = params;

    /* 简化示例：保留接口参数，但不映射 name/priority 语义 */
    (void)name;
    (void)priority;

    pthread_attr_init(&attr);

    /* 简化映射：仅在给定 stackDepth 时尝试设置栈大小，失败则回退默认栈 */
    if (stackDepth > 0U) {
        (void)pthread_attr_setstacksize(&attr, (size_t)stackDepth);
    }

    ret = pthread_create(&thread, &attr, task_entry, wrapper);
    pthread_attr_destroy(&attr);

    if (ret != 0) {
        free(wrapper);
        return pdFAIL;
    }

    if (handle != NULL) {
        pthread_t *stored = (pthread_t *)malloc(sizeof(pthread_t));
        if (stored == NULL) {
            (void)pthread_detach(thread);
            return pdFAIL;
        }
        *stored = thread;
        *handle = (TaskHandle_t)stored;
    } else {
        /* 无句柄场景直接 detach，避免资源泄漏 */
        (void)pthread_detach(thread);
    }

    return pdPASS;
}

void xTaskDelete(TaskHandle_t handle)
{
    if (handle == NULL) {
        pthread_exit(NULL);
    }

    pthread_t *stored = (pthread_t *)handle;
    if (pthread_equal(*stored, pthread_self()) != 0) {
        free(stored);
        pthread_exit(NULL);
    }

    (void)pthread_cancel(*stored);
    (void)pthread_join(*stored, NULL);
    free(stored);
}
