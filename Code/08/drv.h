#ifndef DRV_H
#define DRV_H

#include <stdint.h>

#include "dev.h"

/* 驱动对象：通过 compatible 参与匹配，probe 完成实例绑定 */
typedef struct driver {
    const char *name;
    const char *compatible;
    bus_t *bus;
    int (*probe)(device_t *dev);
} driver_t;

/* SPI 设备描述参数（来自设备配置） */
typedef struct spi_cfg {
    int bus_id;
    int cs;
    uint32_t max_hz;
    uint8_t mode;
} spi_cfg_t;

/* UART 设备描述参数（来自设备配置） */
typedef struct uart_cfg {
    uint32_t base;
    int irq;
    uint32_t baud;
} uart_cfg_t;

/* SPI 驱动私有上下文（probe 后写入 device->driver_data） */
typedef struct spi_priv {
    int cs;
    uint8_t mode;
    uint32_t max_hz;
    const char *kind;
} spi_priv_t;

/* UART 驱动私有上下文 */
typedef struct uart_priv {
    uint32_t base;
    int irq;
    uint32_t baud;
} uart_priv_t;

/* 对外导出的驱动与操作表 */
extern const dev_ops_t spi_ops;
extern driver_t spi_flash_driver;
extern driver_t spi_lcd_driver;
extern driver_t uart_driver;

#endif
