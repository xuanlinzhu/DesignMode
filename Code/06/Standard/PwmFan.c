#include "PwmFan.h"
#include <stdio.h>
#include <windows.h>

typedef struct {
    Actuator base;
    int pwmFrequency;
} PwmFan;

/* 静态实例 */
static PwmFan pwmFanInstance;

static const char* fanBlades[] = {
    "│",
    "/", 
    "─",
    "\\"
};

static void pwmFanInit(Actuator* self) {
    (void)self;
    printf("[PWM Fan] 初始化PWM调速风扇\n");
    printf("[PWM Fan] PWM频率: 25kHz, 调速范围: 0-100%%\n");
}

static void pwmFanSetSpeed(Actuator* self, int speed) {
    PwmFan* fan = (PwmFan*)self;
    if (speed < 0) speed = 0;
    if (speed > 100) speed = 100;
    fan->base.currentSpeed = speed;
    fan->base.isRunning = (speed > 0) ? 1 : 0;
    printf("[PWM Fan] 设置PWM占空比: %d%%\n", speed);
}

static void pwmFanTurnOn(Actuator* self) {
    pwmFanSetSpeed(self, 100);
}

static void pwmFanTurnOff(Actuator* self) {
    pwmFanSetSpeed(self, 0);
}

static int pwmFanGetSpeed(Actuator* self) {
    PwmFan* fan = (PwmFan*)self;
    return fan->base.currentSpeed;
}

static void pwmFanShowStatus(Actuator* self) {
    PwmFan* fan = (PwmFan*)self;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    if (fan->base.isRunning) {
        int bladeIndex = (GetTickCount() / (101 - fan->base.currentSpeed)) % 4;
        SetConsoleTextAttribute(hConsole, 11);
        printf("[PWM Fan] ╔════════════════════╗\n");
        printf("[PWM Fan] ║   %s PWM风扇       ║\n", fanBlades[bladeIndex]);
        printf("[PWM Fan] ║  ┌────────────┐    ║\n");
        printf("[PWM Fan] ║  │▓▓▓▓▓▓▓▓▓▓│    ║  状态: 运行中\n");
        printf("[PWM Fan] ║  │▓▓▓▓▓▓▓▓▓▓│    ║  转速: %d%%\n", fan->base.currentSpeed);
        printf("[PWM Fan] ║  └────────────┘    ║  PWM: %dkHz\n", fan->pwmFrequency);
        printf("[PWM Fan] ╚════════════════════╝\n");
        SetConsoleTextAttribute(hConsole, 7);
    } else {
        SetConsoleTextAttribute(hConsole, 8);
        printf("[PWM Fan] ╔════════════════════╗\n");
        printf("[PWM Fan] ║   ○ PWM风扇        ║\n");
        printf("[PWM Fan] ║      ░░░░░░        ║  状态: 停止\n");
        printf("[PWM Fan] ║      ░░░░░░        ║  转速: 0%%\n");
        printf("[PWM Fan] ╚════════════════════╝\n");
        SetConsoleTextAttribute(hConsole, 7);
    }
}

static void pwmFanDestroy(Actuator* self) {
    (void)self;
    /* 静态分配，无需释放 */
}

Actuator* createPwmFan(void) {
    PwmFan* fan = &pwmFanInstance;

    fan->base.type = "PWM调速风扇";
    fan->base.currentSpeed = 0;
    fan->base.isRunning = 0;
    fan->base.init = pwmFanInit;
    fan->base.setSpeed = pwmFanSetSpeed;
    fan->base.turnOn = pwmFanTurnOn;
    fan->base.turnOff = pwmFanTurnOff;
    fan->base.getSpeed = pwmFanGetSpeed;
    fan->base.showStatus = pwmFanShowStatus;
    fan->base.destroy = pwmFanDestroy;
    fan->pwmFrequency = 25;

    return (Actuator*)fan;
}
