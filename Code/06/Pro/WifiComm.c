#include "WifiComm.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

typedef struct {
    Comm base;
    char ssid[32];
    char ip[16];
    int signalStrength;
} WifiComm;

/* 静态实例 */
static WifiComm wifiCommInstance;

static void wifiInit(Comm* self) {
    WifiComm* wifi = (WifiComm*)self;
    strcpy(wifi->ssid, "SmartHome_5G");
    strcpy(wifi->ip, "192.168.1.100");
    wifi->signalStrength = -45;
    printf("[WiFi Comm] 初始化WiFi通信模块\n");
    printf("[WiFi Comm] 连接网络: %s\n", wifi->ssid);
    printf("[WiFi Comm] IP地址: %s, 信号强度: %d dBm\n", wifi->ip, wifi->signalStrength);
    printf("[WiFi Comm] 支持协议: MQTT, HTTP REST API\n");
}

static int wifiSend(Comm* self, const char* data, int length) {
    (void)self;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 13);
    printf("[WiFi Comm] >>> MQTT发布 [%d字节]\n", length);
    printf("[WiFi Comm] 主题: smart/thermostat/data\n");
    printf("[WiFi Comm] 负载: ");
    for (int i = 0; i < length && i < 50; i++) {
        printf("%c", data[i]);
    }
    if (length > 50) printf("...");
    printf("\n");
    SetConsoleTextAttribute(hConsole, 7);
    return length;
}

static int wifiReceive(Comm* self, char* buffer, int maxLength) {
    (void)self;
    (void)maxLength;
    const char* cmd = "{\"cmd\":\"set_temp\",\"value\":24.5}\n";
    int len = (int)strlen(cmd);
    if (len < maxLength) {
        strcpy(buffer, cmd);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 10);
        printf("[WiFi Comm] <<< MQTT订阅消息\n");
        printf("[WiFi Comm] 主题: smart/thermostat/control\n");
        printf("[WiFi Comm] 负载: %s", buffer);
        SetConsoleTextAttribute(hConsole, 7);
        return len;
    }
    return 0;
}

static int wifiIsConnected(Comm* self) {
    WifiComm* wifi = (WifiComm*)self;
    return (wifi->signalStrength > -80) ? 1 : 0;
}

static void wifiDestroy(Comm* self) {
    (void)self;
    printf("[WiFi Comm] 断开WiFi连接\n");
    /* 静态分配，无需释放 */
}

Comm* createWifiComm(void) {
    WifiComm* wifi = &wifiCommInstance;

    wifi->base.type = "WiFi智能家居通信";
    wifi->base.init = wifiInit;
    wifi->base.send = wifiSend;
    wifi->base.receive = wifiReceive;
    wifi->base.isConnected = wifiIsConnected;
    wifi->base.destroy = wifiDestroy;
    strcpy(wifi->ssid, "");
    strcpy(wifi->ip, "");
    wifi->signalStrength = 0;

    return (Comm*)wifi;
}
