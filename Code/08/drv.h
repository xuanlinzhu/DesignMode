#ifndef DRV_H
#define DRV_H

#include <stdint.h>

#include "dev.h"

typedef struct driver {
    const char *name;
    const char *compatible;
    bus_t *bus;
    int (*probe)(device_t *dev);
} driver_t;

typedef struct spi_cfg {
    int bus_id;
    int cs;
    uint32_t max_hz;
    uint8_t mode;
} spi_cfg_t;

typedef struct uart_cfg {
    uint32_t base;
    int irq;
    uint32_t baud;
} uart_cfg_t;

typedef struct spi_priv {
    int cs;
    uint8_t mode;
    uint32_t max_hz;
    const char *kind;
} spi_priv_t;

typedef struct uart_priv {
    uint32_t base;
    int irq;
    uint32_t baud;
} uart_priv_t;

extern const dev_ops_t spi_ops;
extern driver_t spi_flash_driver;
extern driver_t spi_lcd_driver;
extern driver_t uart_driver;

#endif
