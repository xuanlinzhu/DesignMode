#ifndef BUS_H
#define BUS_H

#include <stddef.h>
#include <stdbool.h>

#include "dev.h"
#include "drv.h"

/* 总线可容纳的最大设备/驱动数量（教学示例固定上限） */
#define MAX_BUS_DEVICES 16U
#define MAX_BUS_DRIVERS 16U

/* 简化互斥锁：用于演示共享总线访问的串行化 */
typedef struct simple_mutex {
    int locked;
} simple_mutex_t;

/* 总线匹配函数：决定设备与驱动是否可绑定 */
typedef int (*bus_match_fn)(const device_t *dev, const driver_t *drv);

typedef int (*bus_configure_fn)(struct bus *bus, struct device *dev);
typedef int (*bus_xfer_fn)(struct bus *bus,
                           struct device *dev,
                           const void *tx,
                           void *rx,
                           size_t len);

typedef struct bus_ops {
    /* 可选：在事务前切换片选/模式/时钟等控制器参数 */
    bus_configure_fn configure;
    /* 可选：统一总线传输入口（可在内部做加锁） */
    bus_xfer_fn xfer;
} bus_ops_t;

/* 总线对象：管理设备、驱动、匹配关系与可选总线能力 */
typedef struct bus {
    const char *name;
    bus_match_fn match;
    device_t *devices[MAX_BUS_DEVICES];
    size_t device_count;
    driver_t *drivers[MAX_BUS_DRIVERS];
    size_t driver_count;
    simple_mutex_t lock;
    bool has_lock;
    const bus_ops_t *ops;
    void *bus_data;
} bus_t;

extern bus_t platform_bus;

/* 互斥锁接口 */
void mutex_init(simple_mutex_t *lock);
void mutex_lock(simple_mutex_t *lock);
void mutex_unlock(simple_mutex_t *lock);

/* 总线框架核心接口 */
void bus_init(bus_t *bus, const char *name, bus_match_fn match);
int bus_match_compatible(const device_t *dev, const driver_t *drv);
int bus_register_driver(bus_t *bus, driver_t *drv);
int bus_register_device(bus_t *bus, device_t *dev);
/* 无独立物理总线时，走 platform_bus 的便捷绑定入口 */
int bind_nobus_device(device_t *dev, driver_t *drivers[], size_t driver_count);

#endif
