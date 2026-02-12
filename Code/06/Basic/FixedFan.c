#include "FixedFan.h"
#include <stdio.h>
#include <windows.h>

typedef struct {
    Actuator base;
} FixedFan;

/* 静态实例 */
static FixedFan fixedFanInstance;

static void fixedFanInit(Actuator* self) {
    (void)self;
    printf("[Fixed Fan] 初始化固定转速风扇\n");
    printf("[Fixed Fan] 转速: 100%% (固定)\n");
}

static void fixedFanSetSpeed(Actuator* self, int speed) {
    (void)self;
    (void)speed;
    printf("[Fixed Fan] 固定风扇不支持调速 (忽略设置: %d%%)\n", speed);
}

static void fixedFanTurnOn(Actuator* self) {
    FixedFan* fan = (FixedFan*)self;
    fan->base.isRunning = 1;
    fan->base.currentSpeed = 100;
    printf("[Fixed Fan] 风扇启动 [ON]\n");
}

static void fixedFanTurnOff(Actuator* self) {
    FixedFan* fan = (FixedFan*)self;
    fan->base.isRunning = 0;
    fan->base.currentSpeed = 0;
    printf("[Fixed Fan] 风扇停止 [OFF]\n");
}

static int fixedFanGetSpeed(Actuator* self) {
    FixedFan* fan = (FixedFan*)self;
    return fan->base.isRunning ? 100 : 0;
}

static void fixedFanShowStatus(Actuator* self) {
    FixedFan* fan = (FixedFan*)self;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    if (fan->base.isRunning) {
        SetConsoleTextAttribute(hConsole, 11);
        printf("[Fixed Fan] ╔════════════════╗\n");
        printf("[Fixed Fan] ║  ● ● ● 固定转速 ║\n");
        printf("[Fixed Fan] ║    ▓▓▓▓▓▓      ║\n");
        printf("[Fixed Fan] ║   ▓▓▓▓▓▓▓▓     ║  状态: 运行中\n");
        printf("[Fixed Fan] ║    ▓▓▓▓▓▓      ║  转速: 100%%\n");
        printf("[Fixed Fan] ╚════════════════╝\n");
        SetConsoleTextAttribute(hConsole, 7);
    } else {
        SetConsoleTextAttribute(hConsole, 8);
        printf("[Fixed Fan] ╔════════════════╗\n");
        printf("[Fixed Fan] ║  ○ ○ ○ 固定风扇 ║\n");
        printf("[Fixed Fan] ║      ░░        ║\n");
        printf("[Fixed Fan] ║     ░░░░       ║  状态: 停止\n");
        printf("[Fixed Fan] ║      ░░        ║  转速: 0%%\n");
        printf("[Fixed Fan] ╚════════════════╝\n");
        SetConsoleTextAttribute(hConsole, 7);
    }
}

static void fixedFanDestroy(Actuator* self) {
    (void)self;
    /* 静态分配，无需释放 */
}

Actuator* createFixedFan(void) {
    FixedFan* fan = &fixedFanInstance;

    fan->base.type = "固定转速风扇";
    fan->base.currentSpeed = 0;
    fan->base.isRunning = 0;
    fan->base.init = fixedFanInit;
    fan->base.setSpeed = fixedFanSetSpeed;
    fan->base.turnOn = fixedFanTurnOn;
    fan->base.turnOff = fixedFanTurnOff;
    fan->base.getSpeed = fixedFanGetSpeed;
    fan->base.showStatus = fixedFanShowStatus;
    fan->base.destroy = fixedFanDestroy;

    return (Actuator*)fan;
}
