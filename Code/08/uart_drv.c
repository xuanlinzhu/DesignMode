#include "drv.h"
#include "bus.h"
#include "cstd.h"

#define MAX_UART_INSTANCES 8U

static uart_priv_t uart_priv_pool[MAX_UART_INSTANCES];
static device_t *uart_bound_devs[MAX_UART_INSTANCES];

static int uart_find_or_alloc_slot(device_t *dev) {
    size_t i = 0U;
    size_t free_idx = MAX_UART_INSTANCES;

    for (i = 0U; i < MAX_UART_INSTANCES; ++i) {
        if (uart_bound_devs[i] == dev) {
            return (int)i;
        }
        if (uart_bound_devs[i] == NULL && free_idx == MAX_UART_INSTANCES) {
            free_idx = i;
        }
    }

    if (free_idx == MAX_UART_INSTANCES) {
        return -1;
    }

    uart_bound_devs[free_idx] = dev;
    return (int)free_idx;
}

static int uart_open(device_t *dev) {
    if (dev == NULL || dev->driver_data == NULL) {
        return -1;
    }
    printf("[UART] open %s\n", dev->name);
    return 0;
}

static int uart_close(device_t *dev) {
    if (dev == NULL || dev->driver_data == NULL) {
        return -1;
    }
    printf("[UART] close %s\n", dev->name);
    return 0;
}

static ssize_t uart_read(device_t *dev, void *buf, size_t n) {
    if (dev == NULL || dev->driver_data == NULL || buf == NULL) {
        return -1;
    }
    memset(buf, 'U', n);
    printf("[UART] read %s %u bytes\n", dev->name, (unsigned int)n);
    return (ssize_t)n;
}

static ssize_t uart_write(device_t *dev, const void *buf, size_t n) {
    (void)buf;
    if (dev == NULL || dev->driver_data == NULL) {
        return -1;
    }
    printf("[UART] write %s %u bytes\n", dev->name, (unsigned int)n);
    return (ssize_t)n;
}

static int uart_ioctl(device_t *dev, int cmd, void *arg) {
    (void)arg;
    if (dev == NULL || dev->driver_data == NULL) {
        return -1;
    }
    printf("[UART] ioctl %s cmd=%d\n", dev->name, cmd);
    return 0;
}

static const dev_ops_t uart_ops = {
    .open = uart_open,
    .close = uart_close,
    .read = uart_read,
    .write = uart_write,
    .ioctl = uart_ioctl
};

static int uart_probe(device_t *dev) {
    const uart_cfg_t *cfg = NULL;
    int slot = -1;

    if (dev == NULL || dev->cfg == NULL) {
        return -1;
    }

    cfg = (const uart_cfg_t *)dev->cfg;
    slot = uart_find_or_alloc_slot(dev);
    if (slot < 0) {
        return -1;
    }

    uart_priv_pool[slot].base = cfg->base;
    uart_priv_pool[slot].irq = cfg->irq;
    uart_priv_pool[slot].baud = cfg->baud;

    dev->driver_data = &uart_priv_pool[slot];
    dev->ops = &uart_ops;

    return 0;
}

driver_t uart_driver = {
    .name = "uart_driver",
    .compatible = "acme,uart",
    .bus = &platform_bus,
    .probe = uart_probe
};
