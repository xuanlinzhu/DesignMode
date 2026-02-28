#ifndef PROTOCOL_BUILDER_H
#define PROTOCOL_BUILDER_H

#include <stddef.h>
#include <stdint.h>

/* 整帧缓冲区上限与负载上限 */
#define FRAME_BUFFER_CAPACITY 256U
#define MAX_PAYLOAD_CAPACITY 160U

/* 构建状态机：约束构建步骤顺序 */
typedef enum FrameBuildState {
    FRAME_STATE_INIT = 0,
    FRAME_STATE_PAYLOAD_READY,
    FRAME_STATE_SUBHEADER_READY,
    FRAME_STATE_SECURITY_READY,
    FRAME_STATE_EXT_READY,
    FRAME_STATE_HEADER_READY,
    FRAME_STATE_CRC_READY
} FrameBuildState;

/* 最终产物：连续字节流 + 长度 */
typedef struct FrameProduct {
    uint8_t bytes[FRAME_BUFFER_CAPACITY];
    size_t length;
} FrameProduct;

/* 构建输入：描述本次要生成的报文参数 */
typedef struct BuildInput {
    uint8_t version;
    uint8_t message_type;
    uint8_t device_id;
    uint8_t flags;
    uint16_t sequence;
    const uint8_t *payload_data;
    uint8_t payload_len;
} BuildInput;

typedef struct FrameBuilder FrameBuilder;

typedef int (*BuildStepFn)(FrameBuilder *, const BuildInput *);

/* Builder：保存中间字段并绑定各构建步骤 */
struct FrameBuilder {
    FrameBuildState state;
    uint8_t version;
    uint8_t payload[MAX_PAYLOAD_CAPACITY];
    size_t payload_len;
    uint8_t sub_header[4];
    size_t sub_header_len;
    uint8_t security_block[8];
    size_t security_len;
    uint8_t ext_header[6];
    size_t ext_len;
    uint8_t header[8];
    size_t header_len;
    uint8_t crc[2];
    size_t crc_len;
    FrameProduct product;

    BuildStepFn reset;
    BuildStepFn build_payload;
    BuildStepFn build_sub_header;
    BuildStepFn build_security;
    BuildStepFn build_ext_header;
    BuildStepFn build_header;
    BuildStepFn build_crc;
};

/* Director：统一调度构建顺序 */
typedef struct FrameDirector {
    int (*construct_frame)(struct FrameDirector *, FrameBuilder *, const BuildInput *);
} FrameDirector;

/* 对外初始化与结果获取接口 */
void init_v1_builder(FrameBuilder *builder);
void init_secure_extended_builder(FrameBuilder *builder);
void init_director(FrameDirector *director);
const FrameProduct *get_frame_product(const FrameBuilder *builder);

#endif
