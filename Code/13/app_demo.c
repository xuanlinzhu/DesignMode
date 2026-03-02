#include "protocol_modbus.h"
#include "protocol_file_transfer.h"
#include "phy_tcp.h"
#include "phy_uart.h"

#include <stdio.h>

/* Modbus 演示：固定协议，注入不同传输实现 */
static void run_demo(protocol_t *protocol, phy_t *phy)
{
    static const uint8_t payload[] = {0x00, 0x00, 0x00, 0x01};
    uint8_t rx_buf[64];
    size_t rx_len = 0U;
    size_t i;

    /* Bridge 组装点：运行期把协议与具体传输方式连接起来 */
    protocol->phy = phy;

    /* 按协议接口顺序执行：init -> tx -> rx */
    if (protocol->ops->init(protocol) != 0) {
        printf("protocol init failed\n");
        return;
    }

    if (protocol->ops->tx(protocol, payload, sizeof(payload)) != 0) {
        printf("protocol tx failed\n");
        (void)protocol->phy->ops->close(protocol->phy);
        return;
    }

    if (protocol->ops->rx(protocol, rx_buf, sizeof(rx_buf), &rx_len) != 0) {
        printf("protocol rx failed\n");
        (void)protocol->phy->ops->close(protocol->phy);
        return;
    }

    /* 示例中仅打印解析后的应答字节 */
    printf("[MODBUS] parsed response:");
    for (i = 0; i < rx_len; ++i) {
        printf(" %02X", rx_buf[i]);
    }
    printf("\n");

    (void)protocol->phy->ops->close(protocol->phy);
}

static void run_file_transfer_demo(protocol_t *protocol, phy_t *phy)
{
    static const uint8_t payload[] = {0x10, 0x20, 0x30, 0x40, 0x50};
    uint8_t rx_buf[64];
    size_t rx_len = 0U;
    size_t i;

    /* 同一套文件传输协议可复用不同 phy 实现 */
    protocol->phy = phy;

    /* 按协议接口顺序执行：init -> tx -> rx */
    if (protocol->ops->init(protocol) != 0) {
        printf("file transfer init failed\n");
        return;
    }

    if (protocol->ops->tx(protocol, payload, sizeof(payload)) != 0) {
        printf("file transfer tx failed\n");
        (void)protocol->phy->ops->close(protocol->phy);
        return;
    }

    if (protocol->ops->rx(protocol, rx_buf, sizeof(rx_buf), &rx_len) != 0) {
        printf("file transfer rx failed\n");
        (void)protocol->phy->ops->close(protocol->phy);
        return;
    }

    printf("[FILE_TRANSFER] parsed response:");
    for (i = 0; i < rx_len; ++i) {
        printf(" %02X", rx_buf[i]);
    }
    printf("\n");

    (void)protocol->phy->ops->close(protocol->phy);
}

int main(void)
{
    protocol_t *modbus;
    protocol_t *file_transfer;
    phy_t *uart;
    phy_t *tcp;

    /* 创建两个协议对象与两种传输对象 */
    modbus = modbus_protocol_create();
    file_transfer = file_transfer_protocol_create();
    uart = phy_uart_create();
    tcp = phy_tcp_create();

    /* 任一对象创建失败则统一回收并退出 */
    if (modbus == NULL || file_transfer == NULL || uart == NULL || tcp == NULL) {
        printf("create instance failed\n");
        modbus_protocol_destroy(modbus);
        file_transfer_protocol_destroy(file_transfer);
        phy_uart_destroy(uart);
        phy_tcp_destroy(tcp);
        return 1;
    }

    /* 协议维度固定，切换传输维度 */
    printf("=== Modbus over UART ===\n");
    run_demo(modbus, uart);

    printf("=== Modbus over TCP ===\n");
    run_demo(modbus, tcp);

    /* 再切换协议维度，继续复用同一组传输实现 */
    printf("=== FileTransfer over UART ===\n");
    run_file_transfer_demo(file_transfer, uart);

    printf("=== FileTransfer over TCP ===\n");
    run_file_transfer_demo(file_transfer, tcp);

    modbus_protocol_destroy(modbus);
    file_transfer_protocol_destroy(file_transfer);
    phy_uart_destroy(uart);
    phy_tcp_destroy(tcp);
    return 0;
}
