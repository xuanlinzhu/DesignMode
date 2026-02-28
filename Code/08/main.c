#include "bus.h"
#include "cstd.h"

/* 对单个设备执行一轮统一访问流程 */
static void exercise_device(const char *title, device_t *dev) {
    char rx_buf[16];
    const char tx_buf[] = "hello";

    printf("\n== %s ==\n", title);
    device_open(dev);
    device_write(dev, tx_buf, 5U);
    device_read(dev, rx_buf, sizeof(rx_buf));
    device_ioctl(dev, 1, NULL);
    device_close(dev);
}

/* 演示 SPI 共享总线 + platform(UART) 逻辑总线的注册与访问 */
static void demo_spi_and_uart(void) {
    bus_t spi_bus;
    bus_t *platform = &platform_bus;

    /* SPI 设备配置：bus_id, cs, max_hz, mode */
    spi_cfg_t flash_cfg0 = {1, 0, 20000000U, 0U};
    spi_cfg_t flash_cfg2 = {1, 2, 15000000U, 0U};
    spi_cfg_t lcd_cfg1 = {1, 1, 10000000U, 3U};
    spi_cfg_t lcd_cfg3 = {1, 3, 8000000U, 3U};
    /* UART 设备配置：base, irq, baud */
    uart_cfg_t uart1_cfg = {0x40011000U, 37, 115200U};
    uart_cfg_t uart2_cfg = {0x40004400U, 38, 9600U};

    /* 设备实例：name, compatible, bus, ops, driver_data, cfg */
    device_t spi_flash_dev0 = {"spi1.0", "acme,spi-flash", NULL, NULL, NULL, &flash_cfg0};
    device_t spi_flash_dev2 = {"spi1.2", "acme,spi-flash", NULL, NULL, NULL, &flash_cfg2};
    device_t spi_lcd_dev1 = {"spi1.1", "acme,spi-lcd", NULL, NULL, NULL, &lcd_cfg1};
    device_t spi_lcd_dev3 = {"spi1.3", "acme,spi-lcd", NULL, NULL, NULL, &lcd_cfg3};
    device_t uart1_dev = {"uart1", "acme,uart", NULL, NULL, NULL, &uart1_cfg};
    device_t uart2_dev = {"uart2", "acme,uart", NULL, NULL, NULL, &uart2_cfg};

    bus_init(&spi_bus, "spi", bus_match_compatible);
    bus_init(platform, "platform", bus_match_compatible);

    /* 混合演示先设备后驱动、先驱动后设备两种注册顺序 */
    if (bus_register_device(&spi_bus, &spi_flash_dev0) != 0) {
        printf("register device %s failed\n", spi_flash_dev0.name);
        return;
    }
    if (bus_register_driver(&spi_bus, &spi_flash_driver) != 0) {
        printf("register driver %s failed\n", spi_flash_driver.name);
        return;
    }
    if (bus_register_driver(&spi_bus, &spi_lcd_driver) != 0) {
        printf("register driver %s failed\n", spi_lcd_driver.name);
        return;
    }
    if (bus_register_device(&spi_bus, &spi_flash_dev2) != 0) {
        printf("register device %s failed\n", spi_flash_dev2.name);
        return;
    }
    if (bus_register_device(&spi_bus, &spi_lcd_dev1) != 0) {
        printf("register device %s failed\n", spi_lcd_dev1.name);
        return;
    }
    if (bus_register_device(&spi_bus, &spi_lcd_dev3) != 0) {
        printf("register device %s failed\n", spi_lcd_dev3.name);
        return;
    }

    if (bus_register_driver(platform, &uart_driver) != 0) {
        printf("register driver %s failed\n", uart_driver.name);
        return;
    }
    if (bus_register_device(platform, &uart1_dev) != 0) {
        printf("register device %s failed\n", uart1_dev.name);
        return;
    }
    if (bus_register_device(platform, &uart2_dev) != 0) {
        printf("register device %s failed\n", uart2_dev.name);
        return;
    }

    exercise_device("SPI flash access: spi1.0", &spi_flash_dev0);
    exercise_device("SPI flash access: spi1.2", &spi_flash_dev2);
    exercise_device("SPI lcd access: spi1.1", &spi_lcd_dev1);
    exercise_device("SPI lcd access: spi1.3", &spi_lcd_dev3);
    exercise_device("UART access (platform bus): uart1", &uart1_dev);
    exercise_device("UART access (platform bus): uart2", &uart2_dev);
}

int main(void) {
    printf("Factory Method demo: simplified driver model\n");
    demo_spi_and_uart();
    return 0;
}
