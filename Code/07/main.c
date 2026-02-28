#include <stdio.h>
#include <string.h>

#include "protocol_builder.h"

/* 按十六进制打印构建结果 */
static void print_hex_frame(const char *title, const FrameProduct *product) {
    size_t i = 0U;
    if (title == NULL || product == NULL) {
        return;
    }

    printf("\n%s\n", title);
    printf("Length: %u bytes\n", (unsigned int)product->length);
    printf("Frame: ");
    for (i = 0U; i < product->length; ++i) {
        printf("%02X", product->bytes[i]);
        if (i + 1U < product->length) {
            printf(" ");
        }
    }
    printf("\n");
}

/* 运行单个 Builder 用例并输出产物 */
static int run_demo_case(const char *name, FrameBuilder *builder, const BuildInput *input) {
    FrameDirector director;
    const FrameProduct *product = NULL;

    if (name == NULL || builder == NULL || input == NULL) {
        return -1;
    }

    init_director(&director);
    if (director.construct_frame(&director, builder, input) != 0) {
        printf("%s: frame build failed\n", name);
        return -1;
    }

    product = get_frame_product(builder);
    if (product == NULL) {
        printf("%s: no frame product\n", name);
        return -1;
    }

    print_hex_frame(name, product);
    return 0;
}

int main(void) {
    FrameBuilder v1_builder;
    FrameBuilder secure_builder;
    uint8_t payload1[] = {0x10U, 0x01U, 0x22U, 0x33U, 0x44U};
    uint8_t payload2[] = {0x20U, 0x02U, 0xAAU, 0xBBU, 0xCCU, 0xDDU};
    BuildInput basic_input;
    BuildInput secure_input;

    /* V1 输入参数 */
    basic_input.version = 1U;
    basic_input.message_type = 0x11U;
    basic_input.device_id = 0x2AU;
    basic_input.flags = 0x01U;
    basic_input.sequence = 0x0102U;
    basic_input.payload_data = payload1;
    basic_input.payload_len = (uint8_t)sizeof(payload1);

    /* 安全扩展输入参数 */
    secure_input.version = 2U;
    secure_input.message_type = 0x31U;
    secure_input.device_id = 0x2AU;
    secure_input.flags = 0x05U;
    secure_input.sequence = 0x0203U;
    secure_input.payload_data = payload2;
    secure_input.payload_len = (uint8_t)sizeof(payload2);

    /* 初始化两类具体 Builder */
    init_v1_builder(&v1_builder);
    init_secure_extended_builder(&secure_builder);

    printf("Builder pattern demo: nested protocol frame construction\n");
    printf("Build order: payload -> sub-header -> security -> ext -> header -> crc\n");

    if (run_demo_case("[V1 builder]", &v1_builder, &basic_input) != 0) {
        return 1;
    }
    if (run_demo_case("[Secure+Extended builder]", &secure_builder, &secure_input) != 0) {
        return 1;
    }

    return 0;
}
