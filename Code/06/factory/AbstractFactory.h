#ifndef ABSTRACT_FACTORY_H
#define ABSTRACT_FACTORY_H

#include "Display.h"
#include "Sensor.h"
#include "Comm.h"
#include "Actuator.h"

/**
 * 温控器版本枚举
 */
typedef enum {
    VERSION_BASIC = 0,
    VERSION_STANDARD = 1,
    VERSION_PRO = 2,
    VERSION_COUNT
} ThermostatVersion;

/**
 * 温控器模块集合结构
 * 包含一个版本的所有模块
 */
typedef struct {
    const char* versionName;    // 版本名称
    Display* display;           // 显示模块
    Sensor* sensor;             // 传感器模块
    Comm* comm;                 // 通信模块
    Actuator* actuator;         // 执行器模块
} ThermostatModuleSet;

/**
 * 抽象工厂接口类型
 */
typedef struct AbstractFactory AbstractFactory;

struct AbstractFactory {
    const char* name;  // 工厂名称
    
    /**
     * 创建模块集合
     * @return 包含所有模块的结构体指针
     */
    ThermostatModuleSet* (*createModuleSet)(AbstractFactory* self);
    
    /**
     * 销毁工厂资源
     */
    void (*destroy)(AbstractFactory* self);
};

/**
 * 创建工厂实例
 * 根据版本号返回对应的工厂实例
 * @param version 温控器版本
 * @return 对应版本的工厂实例，失败返回NULL
 */
AbstractFactory* createFactory(ThermostatVersion version);

/**
 * 获取版本名称
 * @param version 版本枚举值
 * @return 版本名称字符串
 */
const char* getVersionName(ThermostatVersion version);

/**
 * 销毁模块集合
 * @param moduleSet 要销毁的模块集合
 */
void destroyModuleSet(ThermostatModuleSet* moduleSet);

/**
 * 模块创建函数指针组
 * 各版本工厂提供此结构体的静态实例
 */
typedef struct {
    const char* versionName;                    // 版本名称
    Display* (*createDisplay)(void);            // 创建显示模块
    Sensor* (*createSensor)(void);              // 创建传感器模块
    Comm* (*createComm)(void);                  // 创建通信模块
    Actuator* (*createActuator)(void);          // 创建执行器模块
} ModuleCreators;

/**
 * 工厂注册表条目 - 简化为仅包含创建函数组
 * 版本信息由数组索引和 ModuleCreators.versionName 提供
 */
typedef struct {
    ModuleCreators* creators;       // 模块创建函数组
} FactoryRegistryEntry;

#endif // ABSTRACT_FACTORY_H
