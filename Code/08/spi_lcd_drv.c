#include "drv.h"

/* 支持的 LCD 设备实例上限（静态池实现） */
#define MAX_SPI_LCD_INSTANCES 8U

/* 每个 LCD 设备实例对应一份私有上下文 */
static spi_priv_t spi_lcd_priv_pool[MAX_SPI_LCD_INSTANCES];
static device_t *spi_lcd_bound_devs[MAX_SPI_LCD_INSTANCES];

/* 为设备查找已绑定槽位，或分配新的空槽 */
static int spi_lcd_find_or_alloc_slot(device_t *dev) {
    size_t i = 0U;
    size_t free_idx = MAX_SPI_LCD_INSTANCES;

    for (i = 0U; i < MAX_SPI_LCD_INSTANCES; ++i) {
        if (spi_lcd_bound_devs[i] == dev) {
            return (int)i;
        }
        if (spi_lcd_bound_devs[i] == NULL && free_idx == MAX_SPI_LCD_INSTANCES) {
            free_idx = i;
        }
    }

    if (free_idx == MAX_SPI_LCD_INSTANCES) {
        return -1;
    }

    spi_lcd_bound_devs[free_idx] = dev;
    return (int)free_idx;
}

/* 工厂方法：解析 cfg，构造私有上下文，绑定 ops */
static int spi_lcd_probe(device_t *dev) {
    const spi_cfg_t *cfg = NULL;
    int slot = -1;

    if (dev == NULL || dev->cfg == NULL) {
        return -1;
    }

    cfg = (const spi_cfg_t *)dev->cfg;
    slot = spi_lcd_find_or_alloc_slot(dev);
    if (slot < 0) {
        return -1;
    }

    spi_lcd_priv_pool[slot].cs = cfg->cs;
    spi_lcd_priv_pool[slot].mode = cfg->mode;
    spi_lcd_priv_pool[slot].max_hz = cfg->max_hz;
    spi_lcd_priv_pool[slot].kind = "lcd";

    dev->driver_data = &spi_lcd_priv_pool[slot];
    dev->ops = &spi_ops;

    return 0;
}

/* SPI LCD 驱动对象 */
driver_t spi_lcd_driver = {
    .name = "spi_lcd_driver",
    .compatible = "acme,spi-lcd",
    .bus = NULL,
    .probe = spi_lcd_probe
};
