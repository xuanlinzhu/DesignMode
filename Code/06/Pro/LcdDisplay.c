#include "LcdDisplay.h"
#include <stdio.h>
#include <windows.h>

typedef struct {
    Display base;
    int resolution;
    int backlight;
} LcdDisplay;

/* 静态实例 */
static LcdDisplay lcdDisplayInstance;

static void lcdInit(Display* self) {
    (void)self;
    printf("[LCD Display] 初始化液晶显示屏\n");
    printf("[LCD Display] 分辨率: 128x64, 背光: 可调\n");
}

static void lcdShowValue(Display* self, float value, const char* unit) {
    (void)self;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 11);
    
    printf("[LCD Display] ╔═══════════════════════════╗\n");
    printf("[LCD Display] ║     智能温控器 Pro        ║\n");
    printf("[LCD Display] ╠═══════════════════════════╣\n");
    SetConsoleTextAttribute(hConsole, 15);
    printf("[LCD Display] ║  当前温度: %6.2f %s     ║\n", value, unit);
    SetConsoleTextAttribute(hConsole, 11);
    printf("[LCD Display] ║  湿度: 45%%  模式: 自动    ║\n");
    printf("[LCD Display] ║  WiFi: 已连接  固件: v2.1 ║\n");
    printf("[LCD Display] ╚═══════════════════════════╝\n");
    SetConsoleTextAttribute(hConsole, 7);
}

static void lcdShowStatus(Display* self, const char* message) {
    (void)self;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);
    printf("[LCD Display] ║  系统状态: %-16s ║\n", message);
    SetConsoleTextAttribute(hConsole, 7);
}

static void lcdDestroy(Display* self) {
    (void)self;
    /* 静态分配，无需释放 */
}

Display* createLcdDisplay(void) {
    LcdDisplay* display = &lcdDisplayInstance;

    display->base.type = "液晶LCD显示";
    display->base.init = lcdInit;
    display->base.showValue = lcdShowValue;
    display->base.showStatus = lcdShowStatus;
    display->base.destroy = lcdDestroy;
    display->resolution = 128;
    display->backlight = 100;

    return (Display*)display;
}
