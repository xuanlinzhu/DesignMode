#include "SevenSegDisplay.h"
#include <stdio.h>
#include <windows.h>

typedef struct {
    Display base;
    int brightness;
} SevenSegDisplay;

/* 静态实例 */
static SevenSegDisplay sevenSegDisplayInstance;

static void sevenSegInit(Display* self) {
    (void)self;
    printf("[SevenSeg Display] 初始化数码管显示模块\n");
    printf("[SevenSeg Display] 显示范围: -99 ~ 999, 精度: 0.1\n");
}

static void sevenSegShowValue(Display* self, float value, const char* unit) {
    (void)self;
    (void)unit;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    SetConsoleTextAttribute(hConsole, 14);
    printf("[SevenSeg Display] ┌─────────────────┐\n");
    printf("[SevenSeg Display] │  ┌─┐   ┌─┐   ┌─┐  │\n");
    
    int intPart = (int)value;
    int decPart = (int)((value - intPart) * 10);
    if (decPart < 0) decPart = -decPart;
    
    printf("[SevenSeg Display] │  │%d│ . │%d│   ┌─┐  │  %.1f%s\n", 
           abs(intPart) / 10, decPart, value, unit);
    printf("[SevenSeg Display] │  └─┘   └─┘   │%c│  │\n", 
           (value < 0) ? '-' : 'C');
    printf("[SevenSeg Display] └─────────────────┘\n");
    SetConsoleTextAttribute(hConsole, 7);
}

static void sevenSegShowStatus(Display* self, const char* message) {
    (void)self;
    printf("[SevenSeg Display] 状态: %s\n", message);
}

static void sevenSegDestroy(Display* self) {
    (void)self;
    /* 静态分配，无需释放 */
}

Display* createSevenSegDisplay(void) {
    SevenSegDisplay* display = &sevenSegDisplayInstance;

    display->base.type = "数码管显示";
    display->base.init = sevenSegInit;
    display->base.showValue = sevenSegShowValue;
    display->base.showStatus = sevenSegShowStatus;
    display->base.destroy = sevenSegDestroy;
    display->brightness = 100;

    return (Display*)display;
}
