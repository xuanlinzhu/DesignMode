#include "drv.h"
#include "bus.h"
#include "cstd.h"

static int spi_bus_configure(bus_t *bus, device_t *dev);
static int spi_bus_xfer(bus_t *bus, device_t *dev, const void *tx, void *rx, size_t len);

static const bus_ops_t spi_bus_ops = {
    .configure = spi_bus_configure,
    .xfer = spi_bus_xfer
};

static int ensure_spi_bus_ops(bus_t *bus) {
    if (bus == NULL) {
        return -1;
    }
    if (bus->ops == NULL) {
        bus->ops = &spi_bus_ops;
    }
    bus->has_lock = true;
    return 0;
}

static int spi_bus_configure(bus_t *bus, device_t *dev) {
    const spi_priv_t *priv = NULL;

    if (bus == NULL || dev == NULL || dev->driver_data == NULL) {
        return -1;
    }

    priv = (const spi_priv_t *)dev->driver_data;
    printf("[SPI] configure bus=%s dev=%s cs=%d mode=%u max_hz=%u\n",
           bus->name,
           dev->name,
           priv->cs,
           (unsigned int)priv->mode,
           (unsigned int)priv->max_hz);
    return 0;
}

static int spi_bus_xfer(bus_t *bus, device_t *dev, const void *tx, void *rx, size_t len) {
    const spi_priv_t *priv = NULL;

    if (bus == NULL || dev == NULL || dev->driver_data == NULL) {
        return -1;
    }

    priv = (const spi_priv_t *)dev->driver_data;

    if (bus->has_lock) {
        mutex_lock(&bus->lock);
    }

    if (rx != NULL) {
        printf("[SPI] bus=%s xfer read dev=%s cs=%d mode=%u bytes=%u\n",
               bus->name,
               dev->name,
               priv->cs,
               (unsigned int)priv->mode,
               (unsigned int)len);
        memset(rx, 0xA5, len);
    } else {
        (void)tx;
        printf("[SPI] bus=%s xfer write dev=%s cs=%d mode=%u bytes=%u\n",
               bus->name,
               dev->name,
               priv->cs,
               (unsigned int)priv->mode,
               (unsigned int)len);
    }

    if (bus->has_lock) {
        mutex_unlock(&bus->lock);
    }

    return 0;
}

static int spi_open(device_t *dev) {
    if (dev == NULL || dev->driver_data == NULL) {
        return -1;
    }
    printf("[SPI] open %s\n", dev->name);
    return 0;
}

static int spi_close(device_t *dev) {
    if (dev == NULL || dev->driver_data == NULL) {
        return -1;
    }
    printf("[SPI] close %s\n", dev->name);
    return 0;
}

static ssize_t spi_read(device_t *dev, void *buf, size_t n) {
    if (dev == NULL || dev->bus == NULL || dev->driver_data == NULL || buf == NULL) {
        return -1;
    }

    if (ensure_spi_bus_ops(dev->bus) != 0) {
        return -1;
    }
    if (dev->bus->ops == NULL || dev->bus->ops->configure == NULL || dev->bus->ops->xfer == NULL) {
        return -1;
    }
    if (dev->bus->ops->configure(dev->bus, dev) != 0) {
        return -1;
    }
    if (dev->bus->ops->xfer(dev->bus, dev, NULL, buf, n) != 0) {
        return -1;
    }

    return (ssize_t)n;
}

static ssize_t spi_write(device_t *dev, const void *buf, size_t n) {
    if (dev == NULL || dev->bus == NULL || dev->driver_data == NULL || buf == NULL) {
        return -1;
    }

    if (ensure_spi_bus_ops(dev->bus) != 0) {
        return -1;
    }
    if (dev->bus->ops == NULL || dev->bus->ops->configure == NULL || dev->bus->ops->xfer == NULL) {
        return -1;
    }
    if (dev->bus->ops->configure(dev->bus, dev) != 0) {
        return -1;
    }
    if (dev->bus->ops->xfer(dev->bus, dev, buf, NULL, n) != 0) {
        return -1;
    }

    return (ssize_t)n;
}

static int spi_ioctl(device_t *dev, int cmd, void *arg) {
    (void)arg;
    if (dev == NULL || dev->driver_data == NULL) {
        return -1;
    }
    printf("[SPI] ioctl %s cmd=%d\n", dev->name, cmd);
    return 0;
}

const dev_ops_t spi_ops = {
    .open = spi_open,
    .close = spi_close,
    .read = spi_read,
    .write = spi_write,
    .ioctl = spi_ioctl
};
