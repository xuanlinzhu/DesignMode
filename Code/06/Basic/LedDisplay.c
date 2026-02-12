#include "LedDisplay.h"
#include <stdio.h>
#include <windows.h>

typedef struct {
    Display base;
    int color;
} LedDisplay;

/* 静态实例 */
static LedDisplay ledDisplayInstance;

static void ledInit(Display* self) {
    (void)self;
    printf("[LED Display] 初始化三色LED显示模块\n");
    printf("[LED Display] 支持颜色: 红(高温) 绿(舒适) 蓝(低温)\n");
}

static void ledShowValue(Display* self, float value, const char* unit) {
    (void)self;
    const char* color;
    int colorCode;
    
    if (value > 26.0f) {
        color = "红色(高温警告)";
        colorCode = 12;
    } else if (value < 20.0f) {
        color = "蓝色(低温)";
        colorCode = 9;
    } else {
        color = "绿色(舒适)";
        colorCode = 10;
    }
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorCode);
    printf("[LED Display] ● ");
    SetConsoleTextAttribute(hConsole, 7);
    printf("当前温度: %.1f%s (%s)\n", value, unit, color);
}

static void ledShowStatus(Display* self, const char* message) {
    (void)self;
    printf("[LED Display] 状态: %s\n", message);
}

static void ledDestroy(Display* self) {
    (void)self;
    /* 静态分配，无需释放 */
}

Display* createLedDisplay(void) {
    LedDisplay* display = &ledDisplayInstance;

    display->base.type = "三色LED显示";
    display->base.init = ledInit;
    display->base.showValue = ledShowValue;
    display->base.showStatus = ledShowStatus;
    display->base.destroy = ledDestroy;
    display->color = 0;

    return (Display*)display;
}
