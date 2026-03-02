#ifndef FREERTOS_API_H
#define FREERTOS_API_H

#include <stdint.h>

typedef void (*TaskFunction_t)(void *);
typedef void *TaskHandle_t;
typedef int BaseType_t;

#define pdPASS 1
#define pdFAIL 0

BaseType_t xTaskCreate(TaskFunction_t task,
                       const char *name,
                       uint16_t stackDepth,
                       void *params,
                       uint32_t priority,
                       TaskHandle_t *handle);

void xTaskDelete(TaskHandle_t handle);

#endif
