#include "drv.h"

#define MAX_SPI_FLASH_INSTANCES 8U

static spi_priv_t spi_flash_priv_pool[MAX_SPI_FLASH_INSTANCES];
static device_t *spi_flash_bound_devs[MAX_SPI_FLASH_INSTANCES];

static int spi_flash_find_or_alloc_slot(device_t *dev) {
    size_t i = 0U;
    size_t free_idx = MAX_SPI_FLASH_INSTANCES;

    for (i = 0U; i < MAX_SPI_FLASH_INSTANCES; ++i) {
        if (spi_flash_bound_devs[i] == dev) {
            return (int)i;
        }
        if (spi_flash_bound_devs[i] == NULL && free_idx == MAX_SPI_FLASH_INSTANCES) {
            free_idx = i;
        }
    }

    if (free_idx == MAX_SPI_FLASH_INSTANCES) {
        return -1;
    }

    spi_flash_bound_devs[free_idx] = dev;
    return (int)free_idx;
}

static int spi_flash_probe(device_t *dev) {
    const spi_cfg_t *cfg = NULL;
    int slot = -1;

    if (dev == NULL || dev->cfg == NULL) {
        return -1;
    }

    cfg = (const spi_cfg_t *)dev->cfg;
    slot = spi_flash_find_or_alloc_slot(dev);
    if (slot < 0) {
        return -1;
    }

    spi_flash_priv_pool[slot].cs = cfg->cs;
    spi_flash_priv_pool[slot].mode = cfg->mode;
    spi_flash_priv_pool[slot].max_hz = cfg->max_hz;
    spi_flash_priv_pool[slot].kind = "flash";

    dev->driver_data = &spi_flash_priv_pool[slot];
    dev->ops = &spi_ops;

    return 0;
}

driver_t spi_flash_driver = {
    .name = "spi_flash_driver",
    .compatible = "acme,spi-flash",
    .bus = NULL,
    .probe = spi_flash_probe
};
