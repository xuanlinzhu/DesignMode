#include "dev.h"

int device_open(device_t *dev) {
    if (dev == NULL || dev->ops == NULL || dev->ops->open == NULL) {
        return -1;
    }
    return dev->ops->open(dev);
}

int device_close(device_t *dev) {
    if (dev == NULL || dev->ops == NULL || dev->ops->close == NULL) {
        return -1;
    }
    return dev->ops->close(dev);
}

ssize_t device_read(device_t *dev, void *buf, size_t n) {
    if (dev == NULL || dev->ops == NULL || dev->ops->read == NULL) {
        return -1;
    }
    return dev->ops->read(dev, buf, n);
}

ssize_t device_write(device_t *dev, const void *buf, size_t n) {
    if (dev == NULL || dev->ops == NULL || dev->ops->write == NULL) {
        return -1;
    }
    return dev->ops->write(dev, buf, n);
}

int device_ioctl(device_t *dev, int cmd, void *arg) {
    if (dev == NULL || dev->ops == NULL || dev->ops->ioctl == NULL) {
        return -1;
    }
    return dev->ops->ioctl(dev, cmd, arg);
}
