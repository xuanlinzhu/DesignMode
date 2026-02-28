#include "protocol_builder.h"

#include <string.h>

/* 追加写入产物缓冲区，包含容量保护 */
static int append_bytes(FrameProduct *product, const uint8_t *data, size_t len) {
    if (product == NULL || data == NULL) {
        return -1;
    }
    if (product->length + len > FRAME_BUFFER_CAPACITY) {
        return -1;
    }
    memcpy(product->bytes + product->length, data, len);
    product->length += len;
    return 0;
}

/* V1 版本使用的简单 XOR CRC8 */
static uint8_t crc8_xor(const uint8_t *data, size_t len) {
    uint8_t crc = 0U;
    size_t i = 0U;
    for (i = 0U; i < len; ++i) {
        crc ^= data[i];
    }
    return crc;
}

/* 安全扩展版本使用的 CRC16-CCITT */
static uint16_t crc16_ccitt(const uint8_t *data, size_t len) {
    uint16_t crc = 0xFFFFU;
    size_t i = 0U;
    for (i = 0U; i < len; ++i) {
        uint8_t bit = 0U;
        crc ^= (uint16_t)((uint16_t)data[i] << 8);
        for (bit = 0U; bit < 8U; ++bit) {
            if ((crc & 0x8000U) != 0U) {
                crc = (uint16_t)((crc << 1) ^ 0x1021U);
            } else {
                crc = (uint16_t)(crc << 1);
            }
        }
    }
    return crc;
}

/* 重置 builder 中间状态与缓存 */
static int reset_common(FrameBuilder *builder, const BuildInput *input) {
    (void)input;
    if (builder == NULL) {
        return -1;
    }
    builder->state = FRAME_STATE_INIT;
    builder->payload_len = 0U;
    builder->sub_header_len = 0U;
    builder->security_len = 0U;
    builder->ext_len = 0U;
    builder->header_len = 0U;
    builder->crc_len = 0U;
    builder->product.length = 0U;
    memset(builder->payload, 0, sizeof(builder->payload));
    memset(builder->sub_header, 0, sizeof(builder->sub_header));
    memset(builder->security_block, 0, sizeof(builder->security_block));
    memset(builder->ext_header, 0, sizeof(builder->ext_header));
    memset(builder->header, 0, sizeof(builder->header));
    memset(builder->crc, 0, sizeof(builder->crc));
    memset(builder->product.bytes, 0, sizeof(builder->product.bytes));
    return 0;
}

/* 构建 payload 段 */
static int build_payload_common(FrameBuilder *builder, const BuildInput *input) {
    if (builder == NULL || input == NULL || input->payload_data == NULL) {
        return -1;
    }
    if (builder->state != FRAME_STATE_INIT) {
        return -1;
    }
    if (input->payload_len > MAX_PAYLOAD_CAPACITY) {
        return -1;
    }
    memcpy(builder->payload, input->payload_data, input->payload_len);
    builder->payload_len = input->payload_len;
    builder->state = FRAME_STATE_PAYLOAD_READY;
    return 0;
}

/* 构建子头，包含 payload 长度与序号 */
static int build_sub_header_common(FrameBuilder *builder, const BuildInput *input) {
    if (builder == NULL || input == NULL) {
        return -1;
    }
    if (builder->state != FRAME_STATE_PAYLOAD_READY) {
        return -1;
    }

    builder->sub_header[0] = 0xA1U;
    builder->sub_header[1] = (uint8_t)builder->payload_len;
    builder->sub_header[2] = (uint8_t)((input->sequence >> 8) & 0xFFU);
    builder->sub_header[3] = (uint8_t)(input->sequence & 0xFFU);
    builder->sub_header_len = 4U;
    builder->state = FRAME_STATE_SUBHEADER_READY;
    return 0;
}

/* 无安全块版本 */
static int build_security_none(FrameBuilder *builder, const BuildInput *input) {
    (void)input;
    if (builder == NULL) {
        return -1;
    }
    if (builder->state != FRAME_STATE_SUBHEADER_READY) {
        return -1;
    }
    builder->security_len = 0U;
    builder->state = FRAME_STATE_SECURITY_READY;
    return 0;
}

/* 生成简化签名安全块 */
static int build_security_signature(FrameBuilder *builder, const BuildInput *input) {
    uint8_t checksum = 0U;
    size_t i = 0U;
    if (builder == NULL || input == NULL) {
        return -1;
    }
    if (builder->state != FRAME_STATE_SUBHEADER_READY) {
        return -1;
    }

    for (i = 0U; i < builder->payload_len; ++i) {
        checksum = (uint8_t)(checksum + builder->payload[i]);
    }

    builder->security_block[0] = 0x5AU;
    builder->security_block[1] = 0x02U;
    builder->security_block[2] = (uint8_t)input->device_id;
    builder->security_block[3] = checksum;
    builder->security_len = 4U;
    builder->state = FRAME_STATE_SECURITY_READY;
    return 0;
}

/* 无扩展头版本 */
static int build_ext_none(FrameBuilder *builder, const BuildInput *input) {
    (void)input;
    if (builder == NULL) {
        return -1;
    }
    if (builder->state != FRAME_STATE_SECURITY_READY) {
        return -1;
    }
    builder->ext_len = 0U;
    builder->state = FRAME_STATE_EXT_READY;
    return 0;
}

/* 标准扩展头版本 */
static int build_ext_standard(FrameBuilder *builder, const BuildInput *input) {
    if (builder == NULL || input == NULL) {
        return -1;
    }
    if (builder->state != FRAME_STATE_SECURITY_READY) {
        return -1;
    }
    builder->ext_header[0] = 0xE1U;
    builder->ext_header[1] = 0x02U;
    builder->ext_header[2] = (uint8_t)(input->flags | 0x80U);
    builder->ext_header[3] = 0x01U;
    builder->ext_len = 4U;
    builder->state = FRAME_STATE_EXT_READY;
    return 0;
}

/* 构建主头，填入类型/标志/总长度等 */
static int build_header_common(FrameBuilder *builder, const BuildInput *input) {
    uint16_t total_without_crc = 0U;
    if (builder == NULL || input == NULL) {
        return -1;
    }
    if (builder->state != FRAME_STATE_EXT_READY) {
        return -1;
    }

    total_without_crc = (uint16_t)(8U + builder->ext_len + builder->security_len + builder->sub_header_len + builder->payload_len);

    builder->header[0] = 0x7EU;
    builder->header[1] = builder->version;
    builder->header[2] = input->message_type;
    builder->header[3] = input->flags;
    builder->header[4] = (uint8_t)((total_without_crc >> 8) & 0xFFU);
    builder->header[5] = (uint8_t)(total_without_crc & 0xFFU);
    builder->header[6] = input->device_id;
    builder->header[7] = 0x00U;
    builder->header_len = 8U;
    builder->state = FRAME_STATE_HEADER_READY;
    return 0;
}

/* 打包全部字段并追加 CRC8 */
static int build_crc8_and_pack(FrameBuilder *builder, const BuildInput *input) {
    uint8_t c = 0U;
    int rc = 0;
    (void)input;
    if (builder == NULL) {
        return -1;
    }
    if (builder->state != FRAME_STATE_HEADER_READY) {
        return -1;
    }

    rc = append_bytes(&builder->product, builder->header, builder->header_len);
    if (rc != 0) {
        return -1;
    }
    rc = append_bytes(&builder->product, builder->ext_header, builder->ext_len);
    if (rc != 0) {
        return -1;
    }
    rc = append_bytes(&builder->product, builder->security_block, builder->security_len);
    if (rc != 0) {
        return -1;
    }
    rc = append_bytes(&builder->product, builder->sub_header, builder->sub_header_len);
    if (rc != 0) {
        return -1;
    }
    rc = append_bytes(&builder->product, builder->payload, builder->payload_len);
    if (rc != 0) {
        return -1;
    }

    c = crc8_xor(builder->product.bytes, builder->product.length);
    builder->crc[0] = c;
    builder->crc_len = 1U;
    rc = append_bytes(&builder->product, builder->crc, builder->crc_len);
    if (rc != 0) {
        return -1;
    }

    builder->state = FRAME_STATE_CRC_READY;
    return 0;
}

/* 打包全部字段并追加 CRC16 */
static int build_crc16_and_pack(FrameBuilder *builder, const BuildInput *input) {
    uint16_t c = 0U;
    int rc = 0;
    (void)input;
    if (builder == NULL) {
        return -1;
    }
    if (builder->state != FRAME_STATE_HEADER_READY) {
        return -1;
    }

    rc = append_bytes(&builder->product, builder->header, builder->header_len);
    if (rc != 0) {
        return -1;
    }
    rc = append_bytes(&builder->product, builder->ext_header, builder->ext_len);
    if (rc != 0) {
        return -1;
    }
    rc = append_bytes(&builder->product, builder->security_block, builder->security_len);
    if (rc != 0) {
        return -1;
    }
    rc = append_bytes(&builder->product, builder->sub_header, builder->sub_header_len);
    if (rc != 0) {
        return -1;
    }
    rc = append_bytes(&builder->product, builder->payload, builder->payload_len);
    if (rc != 0) {
        return -1;
    }

    c = crc16_ccitt(builder->product.bytes, builder->product.length);
    builder->crc[0] = (uint8_t)((c >> 8) & 0xFFU);
    builder->crc[1] = (uint8_t)(c & 0xFFU);
    builder->crc_len = 2U;
    rc = append_bytes(&builder->product, builder->crc, builder->crc_len);
    if (rc != 0) {
        return -1;
    }

    builder->state = FRAME_STATE_CRC_READY;
    return 0;
}

/* Director 固定构建顺序 */
static int director_construct(struct FrameDirector *director, FrameBuilder *builder, const BuildInput *input) {
    (void)director;
    if (builder == NULL || input == NULL) {
        return -1;
    }
    if (builder->reset(builder, input) != 0) {
        return -1;
    }
    if (builder->build_payload(builder, input) != 0) {
        return -1;
    }
    if (builder->build_sub_header(builder, input) != 0) {
        return -1;
    }
    if (builder->build_security(builder, input) != 0) {
        return -1;
    }
    if (builder->build_ext_header(builder, input) != 0) {
        return -1;
    }
    if (builder->build_header(builder, input) != 0) {
        return -1;
    }
    if (builder->build_crc(builder, input) != 0) {
        return -1;
    }
    return 0;
}

/* 初始化 V1 builder：无安全块、无扩展头、CRC8 */
void init_v1_builder(FrameBuilder *builder) {
    if (builder == NULL) {
        return;
    }
    memset(builder, 0, sizeof(*builder));
    builder->version = 0x01U;
    builder->reset = reset_common;
    builder->build_payload = build_payload_common;
    builder->build_sub_header = build_sub_header_common;
    builder->build_security = build_security_none;
    builder->build_ext_header = build_ext_none;
    builder->build_header = build_header_common;
    builder->build_crc = build_crc8_and_pack;
}

/* 初始化安全扩展 builder：有安全块、有扩展头、CRC16 */
void init_secure_extended_builder(FrameBuilder *builder) {
    if (builder == NULL) {
        return;
    }
    memset(builder, 0, sizeof(*builder));
    builder->version = 0x02U;
    builder->reset = reset_common;
    builder->build_payload = build_payload_common;
    builder->build_sub_header = build_sub_header_common;
    builder->build_security = build_security_signature;
    builder->build_ext_header = build_ext_standard;
    builder->build_header = build_header_common;
    builder->build_crc = build_crc16_and_pack;
}

void init_director(FrameDirector *director) {
    if (director == NULL) {
        return;
    }
    director->construct_frame = director_construct;
}

const FrameProduct *get_frame_product(const FrameBuilder *builder) {
    if (builder == NULL || builder->state != FRAME_STATE_CRC_READY) {
        return NULL;
    }
    return &builder->product;
}
