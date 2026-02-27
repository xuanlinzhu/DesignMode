#include "bus.h"
#include "cstd.h"

bus_t platform_bus;

static int platform_bus_ready = 0;

static int probe_if_match(device_t *dev, driver_t *drv, bus_match_fn match) {
    if (dev == NULL || drv == NULL || match == NULL) {
        return -1;
    }
    if (dev->ops != NULL) {
        return 0;
    }
    if (match(dev, drv) == 0) {
        return 0;
    }
    return drv->probe(dev);
}

void mutex_init(simple_mutex_t *lock) {
    if (lock != NULL) {
        lock->locked = 0;
    }
}

void mutex_lock(simple_mutex_t *lock) {
    if (lock == NULL) {
        return;
    }
    while (lock->locked != 0) {
    }
    lock->locked = 1;
}

void mutex_unlock(simple_mutex_t *lock) {
    if (lock != NULL) {
        lock->locked = 0;
    }
}

void bus_init(bus_t *bus, const char *name, bus_match_fn match) {
    if (bus == NULL) {
        return;
    }
    memset(bus, 0, sizeof(*bus));
    bus->name = name;
    bus->match = match;
    bus->has_lock = false;
    bus->ops = NULL;
    bus->bus_data = NULL;
    mutex_init(&bus->lock);
}

int bus_match_compatible(const device_t *dev, const driver_t *drv) {
    if (dev == NULL || drv == NULL) {
        return 0;
    }
    if (dev->compatible == NULL || drv->compatible == NULL) {
        return 0;
    }
    if (strcmp(dev->compatible, drv->compatible) != 0) {
        return 0;
    }
    if (dev->bus != NULL && drv->bus != NULL && dev->bus == drv->bus) {
        return 1;
    }
    return 0;
}

static int bus_has_driver(const bus_t *bus, const driver_t *drv) {
    size_t i = 0U;
    if (bus == NULL || drv == NULL) {
        return 0;
    }
    for (i = 0U; i < bus->driver_count; ++i) {
        if (bus->drivers[i] == drv) {
            return 1;
        }
    }
    return 0;
}

static int bus_has_device(const bus_t *bus, const device_t *dev) {
    size_t i = 0U;
    if (bus == NULL || dev == NULL) {
        return 0;
    }
    for (i = 0U; i < bus->device_count; ++i) {
        if (bus->devices[i] == dev) {
            return 1;
        }
    }
    return 0;
}

int bus_register_driver(bus_t *bus, driver_t *drv) {
    size_t i = 0U;

    if (bus == NULL || drv == NULL || drv->probe == NULL || bus->match == NULL) {
        return -1;
    }
    if (bus_has_driver(bus, drv) != 0) {
        drv->bus = bus;
        return 0;
    }
    if (bus->driver_count >= MAX_BUS_DRIVERS) {
        return -1;
    }

    drv->bus = bus;
    bus->drivers[bus->driver_count++] = drv;

    for (i = 0U; i < bus->device_count; ++i) {
        if (probe_if_match(bus->devices[i], drv, bus->match) != 0) {
            return -1;
        }
    }
    return 0;
}

int bus_register_device(bus_t *bus, device_t *dev) {
    size_t i = 0U;

    if (bus == NULL || dev == NULL || bus->match == NULL) {
        return -1;
    }
    if (bus_has_device(bus, dev) != 0) {
        dev->bus = bus;
        return 0;
    }
    if (bus->device_count >= MAX_BUS_DEVICES) {
        return -1;
    }

    dev->bus = bus;
    bus->devices[bus->device_count++] = dev;

    for (i = 0U; i < bus->driver_count; ++i) {
        if (probe_if_match(dev, bus->drivers[i], bus->match) != 0) {
            return -1;
        }
    }
    return 0;
}

int bind_nobus_device(device_t *dev, driver_t *drivers[], size_t driver_count) {
    size_t i = 0U;

    if (dev == NULL || drivers == NULL) {
        return -1;
    }

    if (platform_bus_ready == 0) {
        bus_init(&platform_bus, "platform", bus_match_compatible);
        platform_bus_ready = 1;
    }

    for (i = 0U; i < driver_count; ++i) {
        driver_t *drv = drivers[i];
        if (drv == NULL) {
            continue;
        }
        if (bus_register_driver(&platform_bus, drv) != 0) {
            return -1;
        }
    }

    if (bus_register_device(&platform_bus, dev) != 0) {
        return -1;
    }

    if (dev->ops == NULL) {
        return -1;
    }
    return 0;
}
