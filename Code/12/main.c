#include "freertos_api.h"

#include <stdio.h>
#include <unistd.h>

typedef struct {
    const char *name;
    int loops;
    int sleep_seconds;
} task_cfg_t;

static void periodic_task(void *arg)
{
    task_cfg_t *cfg = (task_cfg_t *)arg;
    int i;

    for (i = 0; i < cfg->loops; ++i) {
        printf("[%s] tick=%d\n", cfg->name, i + 1);
        sleep(cfg->sleep_seconds);
    }

    printf("[%s] finished normally\n", cfg->name);
}

static void self_delete_task(void *arg)
{
    const char *name = (const char *)arg;
    printf("[%s] start and self delete\n", name);
    xTaskDelete(NULL);
}

int main(void)
{
    BaseType_t rc1;
    BaseType_t rc2;
    BaseType_t rc3;
    TaskHandle_t telemetry_handle = NULL;
    TaskHandle_t worker_handle = NULL;
    TaskHandle_t self_handle = NULL;
    task_cfg_t telemetry_cfg = {"telemetry", 4, 1};
    task_cfg_t worker_cfg = {"worker", 20, 1};

    rc1 = xTaskCreate(periodic_task,
                      "TelemetryTask",
                      0U,
                      &telemetry_cfg,
                      1U,
                      &telemetry_handle);

    rc2 = xTaskCreate(periodic_task,
                      "WorkerTask",
                      0U,
                      &worker_cfg,
                      1U,
                      &worker_handle);

    rc3 = xTaskCreate(self_delete_task,
                      "SelfDeleteTask",
                      0U,
                      (void *)"oneshot",
                      1U,
                      &self_handle);

    if (rc1 != pdPASS || rc2 != pdPASS || rc3 != pdPASS) {
        printf("task create failed\n");
        return 1;
    }

    printf("all tasks created\n");

    sleep(3);
    printf("destroy telemetry handle after task exit\n");
    xTaskDelete(telemetry_handle);

    printf("demo done\n");

    sleep(60);
    return 0;
}
