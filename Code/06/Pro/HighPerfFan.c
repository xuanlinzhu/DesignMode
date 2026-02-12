#include "HighPerfFan.h"
#include <stdio.h>
#include <windows.h>

typedef struct {
    Actuator base;
    int maxRpm;
    int silentMode;
} HighPerfFan;

/* 静态实例 */
static HighPerfFan highPerfFanInstance;

static const char* highPerfBlades[] = {
    "◢ ◣",
    "◣ ◢",
    "◤ ◥",
    "◥ ◤"
};

static void highPerfFanInit(Actuator* self) {
    HighPerfFan* fan = (HighPerfFan*)self;
    fan->maxRpm = 3000;
    fan->silentMode = 0;
    printf("[HighPerf Fan] 初始化高性能风扇\n");
    printf("[HighPerf Fan] 最大转速: %d RPM, 支持静音模式\n", fan->maxRpm);
    printf("[HighPerf Fan] 采用磁悬浮轴承, 寿命: 50000小时\n");
}

static void highPerfFanSetSpeed(Actuator* self, int speed) {
    HighPerfFan* fan = (HighPerfFan*)self;
    if (speed < 0) speed = 0;
    if (speed > 100) speed = 100;
    fan->base.currentSpeed = speed;
    fan->base.isRunning = (speed > 0) ? 1 : 0;
    
    int rpm = (fan->maxRpm * speed) / 100;
    printf("[HighPerf Fan] 设置转速: %d%% (%d RPM)\n", speed, rpm);
    
    if (fan->silentMode && speed > 0) {
        printf("[HighPerf Fan] [静音模式] 降低噪音中...\n");
    }
}

static void highPerfFanTurnOn(Actuator* self) {
    highPerfFanSetSpeed(self, 100);
}

static void highPerfFanTurnOff(Actuator* self) {
    highPerfFanSetSpeed(self, 0);
}

static int highPerfFanGetSpeed(Actuator* self) {
    HighPerfFan* fan = (HighPerfFan*)self;
    return fan->base.currentSpeed;
}

static void highPerfFanShowStatus(Actuator* self) {
    HighPerfFan* fan = (HighPerfFan*)self;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    int rpm = (fan->maxRpm * fan->base.currentSpeed) / 100;
    
    if (fan->base.isRunning) {
        int animIndex = (GetTickCount() / (101 - fan->base.currentSpeed)) % 4;
        SetConsoleTextAttribute(hConsole, 14);
        printf("[HighPerf Fan] ╔════════════════════════════════╗\n");
        printf("[HighPerf Fan] ║    %s 高性能风扇 %s     ║\n", 
               highPerfBlades[animIndex], highPerfBlades[(animIndex+2)%4]);
        printf("[HighPerf Fan] ║  ╔════════════════════════╗    ║\n");
        printf("[HighPerf Fan] ║  ║████████████████████████║    ║  状态: 运行中\n");
        printf("[HighPerf Fan] ║  ║████████████████████████║    ║  转速: %d%%\n", fan->base.currentSpeed);
        printf("[HighPerf Fan] ║  ╚════════════════════════╝    ║  RPM: %d\n", rpm);
        printf("[HighPerf Fan] ║      磁悬浮轴承运转中          ║  模式: %s\n", 
               fan->silentMode ? "静音" : "性能");
        printf("[HighPerf Fan] ╚════════════════════════════════╝\n");
        SetConsoleTextAttribute(hConsole, 7);
    } else {
        SetConsoleTextAttribute(hConsole, 8);
        printf("[HighPerf Fan] ╔════════════════════════════════╗\n");
        printf("[HighPerf Fan] ║      ○ 高性能风扇 待机 ○       ║\n");
        printf("[HighPerf Fan] ║         ░░░░░░░░░░             ║  状态: 停止\n");
        printf("[HighPerf Fan] ║         ░░░░░░░░░░             ║  转速: 0%%\n");
        printf("[HighPerf Fan] ╚════════════════════════════════╝\n");
        SetConsoleTextAttribute(hConsole, 7);
    }
}

static void highPerfFanDestroy(Actuator* self) {
    (void)self;
    /* 静态分配，无需释放 */
}

Actuator* createHighPerfFan(void) {
    HighPerfFan* fan = &highPerfFanInstance;

    fan->base.type = "高性能磁悬浮风扇";
    fan->base.currentSpeed = 0;
    fan->base.isRunning = 0;
    fan->base.init = highPerfFanInit;
    fan->base.setSpeed = highPerfFanSetSpeed;
    fan->base.turnOn = highPerfFanTurnOn;
    fan->base.turnOff = highPerfFanTurnOff;
    fan->base.getSpeed = highPerfFanGetSpeed;
    fan->base.showStatus = highPerfFanShowStatus;
    fan->base.destroy = highPerfFanDestroy;
    fan->maxRpm = 3000;
    fan->silentMode = 0;

    return (Actuator*)fan;
}
