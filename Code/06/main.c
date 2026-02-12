#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "factory/AbstractFactory.h"

void printSeparator(void) {
    printf("\n═══════════════════════════════════════════════════════════\n");
}

void runThermostatDemo(ThermostatVersion version) {
    printSeparator();
    printf("  正在演示 %s 版本温控器\n", getVersionName(version));
    printSeparator();
    
    AbstractFactory* factory = createFactory(version);
    if (!factory) {
        printf("错误: 无法创建工厂\n");
        return;
    }
    
    ThermostatModuleSet* modules = factory->createModuleSet(factory);
    if (!modules) {
        printf("错误: 无法创建模块集\n");
        factory->destroy(factory);
        return;
    }
    
    printf("\n┌─────────────────────────────────────────┐\n");
    printf("│  版本: %-10s                      │\n", modules->versionName);
    printf("│  显示: %-20s          │\n", modules->display->type);
    printf("│  传感器: %-20s         │\n", modules->sensor->type);
    printf("│  通信: %-20s          │\n", modules->comm->type);
    printf("│  执行器: %-20s         │\n", modules->actuator->type);
    printf("└─────────────────────────────────────────┘\n");
    
    printf("\n【初始化模块】\n");
    modules->display->init(modules->display);
    modules->sensor->init(modules->sensor);
    modules->comm->init(modules->comm);
    modules->actuator->init(modules->actuator);
    
    printf("\n【读取温度】\n");
    float temp = modules->sensor->readValue(modules->sensor);
    float precision = modules->sensor->getPrecision(modules->sensor);
    printf("传感器精度: ±%.1f°C\n", precision);
    
    printf("\n【显示温度】\n");
    modules->display->showValue(modules->display, temp, "°C");
    modules->display->showStatus(modules->display, "正常运行");
    
    printf("\n【通信测试】\n");
    char sendData[64];
    snprintf(sendData, sizeof(sendData), "TEMP:%.2f", temp);
    modules->comm->send(modules->comm, sendData, (int)strlen(sendData));
    
    char recvBuffer[128];
    modules->comm->receive(modules->comm, recvBuffer, sizeof(recvBuffer));
    
    printf("\n【风扇控制】\n");
    int targetSpeed = (temp > 25.0f) ? 75 : 30;
    modules->actuator->setSpeed(modules->actuator, targetSpeed);
    modules->actuator->showStatus(modules->actuator);
    
    Sleep(500);
    
    printf("\n【模拟运行】\n");
    for (int i = 0; i < 3; i++) {
        temp = modules->sensor->readValue(modules->sensor);
        printf("  读取温度: %.2f°C -> ", temp);
        
        if (temp > 26.0f) {
            modules->actuator->setSpeed(modules->actuator, 100);
            printf("高温! 风扇全速\n");
        } else if (temp < 20.0f) {
            modules->actuator->setSpeed(modules->actuator, 0);
            printf("低温. 风扇停止\n");
        } else {
            modules->actuator->setSpeed(modules->actuator, 50);
            printf("舒适. 风扇中速\n");
        }
        
        modules->display->showValue(modules->display, temp, "°C");
        Sleep(300);
    }
    
    modules->actuator->showStatus(modules->actuator);
    
    printf("\n【关闭系统】\n");
    modules->actuator->turnOff(modules->actuator);
    modules->actuator->showStatus(modules->actuator);
    
    destroyModuleSet(modules);
    factory->destroy(factory);
}

int main(void) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleOutputCP(CP_UTF8);
    
    SetConsoleTextAttribute(hConsole, 14);
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                                                           ║\n");
    printf("║        智能温控器 - 抽象工厂模式演示                      ║\n");
    printf("║                                                           ║\n");
    printf("║        三种版本: Basic / Standard / Pro                   ║\n");
    printf("║                                                           ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    SetConsoleTextAttribute(hConsole, 7);
    
    printf("\n本演示展示抽象工厂模式的核心思想:\n");
    printf("  1. 同一版本的产品族配套使用\n");
    printf("  2. 客户端只依赖抽象接口\n");
    printf("  3. 切换版本只需改变参数\n");
    printf("  4. 新增版本无需修改现有代码\n");
    
    // runThermostatDemo(VERSION_BASIC);

    
    // runThermostatDemo(VERSION_STANDARD);

    
    runThermostatDemo(VERSION_PRO);
    
    printSeparator();
    SetConsoleTextAttribute(hConsole, 10);
    printf("\n  演示完成!\n");
    printf("  三种版本的温控器均通过抽象工厂模式创建和管理\n");
    printf("  客户端代码无需关心具体实现细节\n\n");
    SetConsoleTextAttribute(hConsole, 7);
    
    printf("按任意键退出...\n");
    getchar();
    
    return 0;
}
