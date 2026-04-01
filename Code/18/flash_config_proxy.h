#ifndef FLASH_CONFIG_PROXY_H
#define FLASH_CONFIG_PROXY_H

#include <stdint.h>

/* 当前配置结构版本号，用于识别文件中的数据是否与程序匹配。 */
#define APP_CONFIG_VERSION 0x00010001u

/* 统一返回值：成功返回 0，失败返回 -1。 */
#define FLASH_PROXY_OK 0
#define FLASH_PROXY_ERR -1

/*
 * 应用配置。
 *
 * 这个结构就是最终被写入“模拟 Flash 文件”的完整内容。
 * 其中 crc32 字段不参与自身的 CRC 计算，只用于保存校验结果。
 */
typedef struct
{
    uint32_t version;
    uint8_t device_id;
    uint8_t mode;
    uint16_t sample_interval_ms;
    uint32_t baudrate;
    uint8_t enable_log;
    uint8_t reserved[3];
    uint32_t crc32;
} AppConfig;

/*
 * Flash 配置代理。
 *
 * 业务层不能直接接触文件，只能通过这个代理访问配置。
 * 代理内部统一管理：
 * 1. cache            当前缓存配置
 * 2. is_initialized   代理是否已初始化
 * 3. is_cache_valid   当前缓存是否可读
 * 4. is_dirty         缓存是否已经被修改但尚未提交到文件
 * 5. storage_path     模拟 Flash 的文件路径
 */
typedef struct
{
    AppConfig cache;
    int is_initialized;
    int is_cache_valid;
    int is_dirty;
    const char *storage_path;
} FlashConfigProxy;

/*
 * 计算配置的 CRC32。
 * 计算范围覆盖 AppConfig 中除 crc32 字段外的全部字节。
 */
uint32_t AppConfig_CalcCrc32(const AppConfig *cfg);

/*
 * 初始化代理。
 *
 * 行为：
 * - 尝试从文件加载配置
 * - version 与 crc32 校验通过则加载到 cache
 * - 否则恢复默认配置，并标记 dirty=1，等待后续显式提交
 */
int FlashConfigProxy_Init(FlashConfigProxy *p, const char *path);

/* 读取整体配置，只从 cache 读取，不访问文件。 */
int FlashConfigProxy_Get(const FlashConfigProxy *p, AppConfig *out);

/*
 * 设置整体配置，只修改 cache。
 * 输入中的 crc32 会被忽略，函数内部会重新计算并刷新 dirty 状态。
 */
int FlashConfigProxy_Set(FlashConfigProxy *p, const AppConfig *in);

/* 单字段读取/写入示例：device_id。 */
int FlashConfigProxy_GetDeviceId(const FlashConfigProxy *p, uint8_t *out);
int FlashConfigProxy_SetDeviceId(FlashConfigProxy *p, uint8_t v);

/* 单字段读取/写入示例：mode。 */
int FlashConfigProxy_GetMode(const FlashConfigProxy *p, uint8_t *out);
int FlashConfigProxy_SetMode(FlashConfigProxy *p, uint8_t v);

/*
 * 提交缓存到文件。
 *
 * 行为：
 * - 重新计算 CRC
 * - 以 "wb" 方式整体覆盖写入文件
 * - 成功后 dirty 清零
 */
int FlashConfigProxy_Commit(FlashConfigProxy *p);

/*
 * 从文件重新加载配置。
 * 若加载失败或校验失败，则恢复默认配置并保持 dirty=1。
 */
int FlashConfigProxy_Reload(FlashConfigProxy *p);

/* 查询当前缓存是否为“已修改未提交”状态。 */
int FlashConfigProxy_IsDirty(const FlashConfigProxy *p);

#endif
