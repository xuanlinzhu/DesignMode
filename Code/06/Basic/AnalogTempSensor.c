#include "AnalogTempSensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    Sensor base;
    float currentValue;
} AnalogTempSensor;

/* 静态实例 */
static AnalogTempSensor analogSensorInstance;

static void analogInit(Sensor* self) {
    (void)self;
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }
    printf("[Analog Sensor] 初始化模拟温度传感器\n");
    printf("[Analog Sensor] 精度: ±2°C, 范围: -10°C ~ 50°C\n");
}

static float analogReadValue(Sensor* self) {
    AnalogTempSensor* sensor = (AnalogTempSensor*)self;
    float noise = ((float)(rand() % 100) - 50.0f) / 10.0f;
    sensor->currentValue = 22.0f + noise;
    return sensor->currentValue;
}

static float analogGetPrecision(Sensor* self) {
    (void)self;
    return 2.0f;
}

static void analogDestroy(Sensor* self) {
    (void)self;
    /* 静态分配，无需释放 */
}

Sensor* createAnalogTempSensor(void) {
    AnalogTempSensor* sensor = &analogSensorInstance;

    sensor->base.type = "模拟温度传感器";
    sensor->base.minValue = -10.0f;
    sensor->base.maxValue = 50.0f;
    sensor->base.init = analogInit;
    sensor->base.readValue = analogReadValue;
    sensor->base.getPrecision = analogGetPrecision;
    sensor->base.destroy = analogDestroy;
    sensor->currentValue = 22.0f;

    return (Sensor*)sensor;
}
