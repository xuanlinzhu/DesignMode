#include "HighPrecTempSensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    Sensor base;
    float calibrationOffset;
} HighPrecTempSensor;

/* 静态实例 */
static HighPrecTempSensor highPrecSensorInstance;

static void highPrecInit(Sensor* self) {
    (void)self;
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }
    printf("[HighPrec Sensor] 初始化高精度数字温度传感器\n");
    printf("[HighPrec Sensor] 精度: ±0.1°C, 范围: -40°C ~ 85°C\n");
    printf("[HighPrec Sensor] 16位ADC, 0.01°C分辨率\n");
}

static float highPrecReadValue(Sensor* self) {
    HighPrecTempSensor* sensor = (HighPrecTempSensor*)self;
    float noise = ((float)(rand() % 100) - 50.0f) / 1000.0f;
    float value = 22.00f + noise + sensor->calibrationOffset;
    return value;
}

static float highPrecGetPrecision(Sensor* self) {
    (void)self;
    return 0.1f;
}

static void highPrecDestroy(Sensor* self) {
    (void)self;
    /* 静态分配，无需释放 */
}

Sensor* createHighPrecTempSensor(void) {
    HighPrecTempSensor* sensor = &highPrecSensorInstance;

    sensor->base.type = "高精度数字温度传感器";
    sensor->base.minValue = -40.0f;
    sensor->base.maxValue = 85.0f;
    sensor->base.init = highPrecInit;
    sensor->base.readValue = highPrecReadValue;
    sensor->base.getPrecision = highPrecGetPrecision;
    sensor->base.destroy = highPrecDestroy;
    sensor->calibrationOffset = 0.0f;

    return (Sensor*)sensor;
}
