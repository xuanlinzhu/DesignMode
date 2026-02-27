#ifndef DEV_H
#define DEV_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#if defined(_MSC_VER) && !defined(_SSIZE_T_DEFINED)
typedef ptrdiff_t ssize_t;
#define _SSIZE_T_DEFINED
#endif

struct device;

typedef struct dev_ops {
    int (*open)(struct device *dev);
    int (*close)(struct device *dev);
    ssize_t (*read)(struct device *dev, void *buf, size_t n);
    ssize_t (*write)(struct device *dev, const void *buf, size_t n);
    int (*ioctl)(struct device *dev, int cmd, void *arg);
} dev_ops_t;

typedef struct bus bus_t;

typedef struct device {
    const char *name;
    const char *compatible;
    bus_t *bus;
    const dev_ops_t *ops;
    void *driver_data;
    const void *cfg;
} device_t;

int device_open(device_t *dev);
int device_close(device_t *dev);
ssize_t device_read(device_t *dev, void *buf, size_t n);
ssize_t device_write(device_t *dev, const void *buf, size_t n);
int device_ioctl(device_t *dev, int cmd, void *arg);

#endif
