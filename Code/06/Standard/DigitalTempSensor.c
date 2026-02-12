#include "DigitalTempSensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    Sensor base;
    float lastValue;
} DigitalTempSensor;

/* 静态实例 */
static DigitalTempSensor digitalSensorInstance;

static void digitalInit(Sensor* self) {
    (void)self;
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }
    printf("[Digital Sensor] 初始化数字温度传感器\n");
    printf("[Digital Sensor] 精度: ±0.5°C, 范围: -20°C ~ 60°C\n");
}

static float digitalReadValue(Sensor* self) {
    DigitalTempSensor* sensor = (DigitalTempSensor*)self;
    float noise = ((float)(rand() % 100) - 50.0f) / 100.0f;
    sensor->lastValue = 22.0f + noise;
    return sensor->lastValue;
}

static float digitalGetPrecision(Sensor* self) {
    (void)self;
    return 0.5f;
}

static void digitalDestroy(Sensor* self) {
    (void)self;
    /* 静态分配，无需释放 */
}

Sensor* createDigitalTempSensor(void) {
    DigitalTempSensor* sensor = &digitalSensorInstance;

    sensor->base.type = "数字温度传感器";
    sensor->base.minValue = -20.0f;
    sensor->base.maxValue = 60.0f;
    sensor->base.init = digitalInit;
    sensor->base.readValue = digitalReadValue;
    sensor->base.getPrecision = digitalGetPrecision;
    sensor->base.destroy = digitalDestroy;
    sensor->lastValue = 22.0f;

    return (Sensor*)sensor;
}
