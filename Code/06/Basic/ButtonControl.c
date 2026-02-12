#include "ButtonControl.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    Comm base;
    int buttonPressed;
} ButtonControl;

/* 静态实例 */
static ButtonControl buttonControlInstance;

static void buttonInit(Comm* self) {
    (void)self;
    printf("[Button Control] 初始化本地按键控制模块\n");
    printf("[Button Control] 支持: 温度+/温度-/模式切换\n");
}

static int buttonSend(Comm* self, const char* data, int length) {
    (void)self;
    (void)length;
    printf("[Button Control] 本地操作: %s\n", data);
    return 0;
}

static int buttonReceive(Comm* self, char* buffer, int maxLength) {
    ButtonControl* btn = (ButtonControl*)self;
    (void)maxLength;
    
    if (btn->buttonPressed) {
        strcpy(buffer, "BUTTON_PRESSED");
        btn->buttonPressed = 0;
        return 14;
    }
    return 0;
}

static int buttonIsConnected(Comm* self) {
    (void)self;
    return 1;
}

static void buttonDestroy(Comm* self) {
    (void)self;
    /* 静态分配，无需释放 */
}

Comm* createButtonControl(void) {
    ButtonControl* btn = &buttonControlInstance;

    btn->base.type = "本地按键控制";
    btn->base.init = buttonInit;
    btn->base.send = buttonSend;
    btn->base.receive = buttonReceive;
    btn->base.isConnected = buttonIsConnected;
    btn->base.destroy = buttonDestroy;
    btn->buttonPressed = 0;

    return (Comm*)btn;
}
