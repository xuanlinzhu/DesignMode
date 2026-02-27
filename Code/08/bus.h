#ifndef BUS_H
#define BUS_H

#include <stddef.h>
#include <stdbool.h>

#include "dev.h"
#include "drv.h"

#define MAX_BUS_DEVICES 16U
#define MAX_BUS_DRIVERS 16U

typedef struct simple_mutex {
    int locked;
} simple_mutex_t;

typedef int (*bus_match_fn)(const device_t *dev, const driver_t *drv);

typedef int (*bus_configure_fn)(struct bus *bus, struct device *dev);
typedef int (*bus_xfer_fn)(struct bus *bus,
                           struct device *dev,
                           const void *tx,
                           void *rx,
                           size_t len);

typedef struct bus_ops {
    bus_configure_fn configure;
    bus_xfer_fn xfer;
} bus_ops_t;

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

void mutex_init(simple_mutex_t *lock);
void mutex_lock(simple_mutex_t *lock);
void mutex_unlock(simple_mutex_t *lock);

void bus_init(bus_t *bus, const char *name, bus_match_fn match);
int bus_match_compatible(const device_t *dev, const driver_t *drv);
int bus_register_driver(bus_t *bus, driver_t *drv);
int bus_register_device(bus_t *bus, device_t *dev);
int bind_nobus_device(device_t *dev, driver_t *drivers[], size_t driver_count);

#endif
