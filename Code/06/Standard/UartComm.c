#include "UartComm.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

typedef struct {
    Comm base;
    int baudRate;
    int connected;
} UartComm;

/* 静态实例 */
static UartComm uartCommInstance;

static void uartInit(Comm* self) {
    UartComm* uart = (UartComm*)self;
    uart->baudRate = 9600;
    uart->connected = 1;
    printf("[UART Comm] 初始化UART通信模块\n");
    printf("[UART Comm] 波特率: %d bps, 数据位: 8, 停止位: 1\n", uart->baudRate);
}

static int uartSend(Comm* self, const char* data, int length) {
    (void)self;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 11);
    printf("[UART Comm] >>> 发送 [%d字节]: ", length);
    for (int i = 0; i < length && i < 32; i++) {
        printf("%c", data[i]);
    }
    if (length > 32) printf("...");
    printf("\n");
    SetConsoleTextAttribute(hConsole, 7);
    return length;
}

static int uartReceive(Comm* self, char* buffer, int maxLength) {
    (void)self;
    (void)maxLength;
    const char* mockData = "TEMP_QUERY\n";
    int len = (int)strlen(mockData);
    if (len < maxLength) {
        strcpy(buffer, mockData);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 10);
        printf("[UART Comm] <<< 接收: %s", buffer);
        SetConsoleTextAttribute(hConsole, 7);
        return len;
    }
    return 0;
}

static int uartIsConnected(Comm* self) {
    UartComm* uart = (UartComm*)self;
    return uart->connected;
}

static void uartDestroy(Comm* self) {
    (void)self;
    /* 静态分配，无需释放 */
}

Comm* createUartComm(void) {
    UartComm* uart = &uartCommInstance;

    uart->base.type = "UART串口通信";
    uart->base.init = uartInit;
    uart->base.send = uartSend;
    uart->base.receive = uartReceive;
    uart->base.isConnected = uartIsConnected;
    uart->base.destroy = uartDestroy;
    uart->baudRate = 9600;
    uart->connected = 0;

    return (Comm*)uart;
}
