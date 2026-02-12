#include "factory/AbstractFactory.h"
#include "Basic/BasicFactory.h"
#include "Standard/StandardFactory.h"
#include "Pro/ProFactory.h"
#include <stdio.h>

/* 工厂注册表 - 静态结构体数组，版本由数组索引决定 */
static FactoryRegistryEntry factoryRegistry[] = {
    { .creators = &BasicModuleCreators },      // VERSION_BASIC = 0
    { .creators = &StandardModuleCreators },   // VERSION_STANDARD = 1
    { .creators = &ProModuleCreators }         // VERSION_PRO = 2
};

/* 静态工厂实例结构 */
typedef struct {
    AbstractFactory base;
    FactoryRegistryEntry* entry;
} FactoryImpl;

/* 静态工厂实例 */
static FactoryImpl factoryImpl;

/* 静态模块集实例 */
static ThermostatModuleSet staticModuleSet;

/* 获取版本名称 - 通过索引查找 */
const char* getVersionName(ThermostatVersion version) {
    if (version >= 0 && version < VERSION_COUNT) {
        return factoryRegistry[version].creators->versionName;
    }
    return "Unknown";
}

/* 根据版本查找注册表条目 */
static FactoryRegistryEntry* findRegistryEntry(ThermostatVersion version) {
    if (version >= 0 && version < VERSION_COUNT) {
        return &factoryRegistry[version];
    }
    return NULL;
}

/* 使用注册表条目创建模块集 - 返回静态实例 */
static ThermostatModuleSet* createModuleSetFromRegistry(FactoryRegistryEntry* entry) {
    if (!entry || !entry->creators) {
        return NULL;
    }

    ThermostatModuleSet* set = &staticModuleSet;

    set->versionName = entry->creators->versionName;

    /* 使用函数指针创建各模块 */
    set->display = entry->creators->createDisplay();
    if (!set->display) {
        printf("[FactoryRegistry] 创建显示模块失败\n");
        return NULL;
    }

    set->sensor = entry->creators->createSensor();
    if (!set->sensor) {
        printf("[FactoryRegistry] 创建传感器模块失败\n");
        return NULL;
    }

    set->comm = entry->creators->createComm();
    if (!set->comm) {
        printf("[FactoryRegistry] 创建通信模块失败\n");
        return NULL;
    }

    set->actuator = entry->creators->createActuator();
    if (!set->actuator) {
        printf("[FactoryRegistry] 创建执行器模块失败\n");
        return NULL;
    }

    printf("[FactoryRegistry] %s版本模块集创建成功\n", entry->creators->versionName);
    return set;
}

static ThermostatModuleSet* factoryCreateModuleSet(AbstractFactory* base) {
    FactoryImpl* impl = (FactoryImpl*)base;
    if (!impl || !impl->entry) {
        return NULL;
    }
    return createModuleSetFromRegistry(impl->entry);
}

static void factoryDestroy(AbstractFactory* base) {
    (void)base;
    /* 静态分配，无需释放 */
}

/* 创建工厂实例 - 查表法，返回静态实例 */
AbstractFactory* createFactory(ThermostatVersion version) {
    FactoryRegistryEntry* entry = findRegistryEntry(version);

    if (!entry) {
        printf("[FactoryRegistry] 错误: 未知版本 %d\n", version);
        return NULL;
    }

    printf("\n========================================\n");
    printf("   创建 %s 版本工厂\n", entry->creators->versionName);
    printf("========================================\n");

    /* 使用静态工厂实例 */
    factoryImpl.base.name = entry->creators->versionName;
    factoryImpl.base.createModuleSet = factoryCreateModuleSet;
    factoryImpl.base.destroy = factoryDestroy;
    factoryImpl.entry = entry;

    return (AbstractFactory*)&factoryImpl;
}

/* 销毁模块集 */
void destroyModuleSet(ThermostatModuleSet* moduleSet) {
    if (!moduleSet) return;

    printf("\n[清理资源] 销毁 %s 版本模块集...\n", moduleSet->versionName);

    if (moduleSet->display) {
        moduleSet->display->destroy(moduleSet->display);
        moduleSet->display = NULL;
    }
    if (moduleSet->sensor) {
        moduleSet->sensor->destroy(moduleSet->sensor);
        moduleSet->sensor = NULL;
    }
    if (moduleSet->comm) {
        moduleSet->comm->destroy(moduleSet->comm);
        moduleSet->comm = NULL;
    }
    if (moduleSet->actuator) {
        moduleSet->actuator->destroy(moduleSet->actuator);
        moduleSet->actuator = NULL;
    }

    /* 静态分配，无需释放 moduleSet 本身 */
    printf("[清理资源] 完成\n");
}
