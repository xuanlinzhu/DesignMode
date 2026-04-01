#include "flash_config_proxy.h"

#include <stddef.h>

typedef struct FILE FILE;
extern FILE *fopen(const char *filename, const char *mode);
extern size_t fread(void *ptr, size_t size, size_t count, FILE *stream);
extern size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);
extern int fclose(FILE *stream);

#define FLASH_STORAGE_DEFAULT_PATH "flash_storage.bin"

/*
 * 载入默认配置。
 *
 * 当文件不存在、文件大小不匹配、版本不匹配或 CRC 校验失败时，
 * 代理都会回退到这份默认配置，并把 dirty 置为 1，表示“缓存有效但尚未落盘”。
 */
static void AppConfig_LoadDefault(AppConfig *cfg)
{
    if (cfg == NULL) {
        return;
    }

    cfg->version = APP_CONFIG_VERSION;
    cfg->device_id = 1u;
    cfg->mode = 0u;
    cfg->sample_interval_ms = 1000u;
    cfg->baudrate = 115200u;
    cfg->enable_log = 1u;
    cfg->reserved[0] = 0u;
    cfg->reserved[1] = 0u;
    cfg->reserved[2] = 0u;
    cfg->crc32 = AppConfig_CalcCrc32(cfg);
}

/*
 * 校验一份配置是否可用。
 *
 * 当前规则只有两条：
 * 1. version 必须与当前程序约定一致
 * 2. crc32 必须等于重新计算的结果
 */
static int AppConfig_IsValid(const AppConfig *cfg)
{
    if (cfg == NULL) {
        return 0;
    }

    if (cfg->version != APP_CONFIG_VERSION) {
        return 0;
    }

    if (cfg->crc32 != AppConfig_CalcCrc32(cfg)) {
        return 0;
    }

    return 1;
}

/*
 * 从模拟 Flash 文件中整体读取一份 AppConfig。
 *
 * 这里额外多读 1 个字节，是为了确认文件大小“恰好等于 sizeof(AppConfig)”。
 * 如果还能继续读出额外字节，说明文件过大，也应判定为非法。
 */
static int FlashStorage_ReadAll(const char *path, AppConfig *out)
{
    FILE *fp;
    AppConfig tmp;
    unsigned char extra_byte;
    size_t read_count;
    size_t extra_read;

    if (path == NULL || out == NULL) {
        return FLASH_PROXY_ERR;
    }

    fp = fopen(path, "rb");
    if (fp == NULL) {
        return FLASH_PROXY_ERR;
    }

    read_count = fread(&tmp, sizeof(AppConfig), 1u, fp);
    extra_read = fread(&extra_byte, 1u, 1u, fp);
    (void)fclose(fp);

    if (read_count != 1u) {
        return FLASH_PROXY_ERR;
    }

    if (extra_read != 0u) {
        return FLASH_PROXY_ERR;
    }

    *out = tmp;
    return FLASH_PROXY_OK;
}

/*
 * 将整份缓存配置覆盖写入模拟 Flash 文件。
 *
 * 写入模式使用 "wb"，符合题目要求，也表示每次 commit 都以缓存内容
 * 完整覆盖旧文件，而不是做局部更新。
 */
static int FlashStorage_WriteAll(const char *path, const AppConfig *in)
{
    FILE *fp;
    size_t write_count;

    if (path == NULL || in == NULL) {
        return FLASH_PROXY_ERR;
    }

    fp = fopen(path, "wb");
    if (fp == NULL) {
        return FLASH_PROXY_ERR;
    }

    write_count = fwrite(in, sizeof(AppConfig), 1u, fp);
    (void)fclose(fp);

    if (write_count != 1u) {
        return FLASH_PROXY_ERR;
    }

    return FLASH_PROXY_OK;
}

/*
 * 计算 AppConfig 的 CRC32。
 *
 * 实现采用常见的 CRC-32（多项式 0xEDB88320）按字节逐位计算。
 * 关键点：
 * - 只覆盖结构体中 crc32 之前的字节
 * - 不把 crc32 字段自身纳入计算
 * - 这样同一份配置可以稳定地“先算 CRC，再把 CRC 写回结构体”
 */
uint32_t AppConfig_CalcCrc32(const AppConfig *cfg)
{
    const unsigned char *bytes;
    uint32_t crc;
    size_t i;
    size_t j;

    if (cfg == NULL) {
        return 0u;
    }

    bytes = (const unsigned char *)cfg;
    crc = 0xFFFFFFFFu;

    for (i = 0u; i < offsetof(AppConfig, crc32); ++i) {
        crc ^= (uint32_t)bytes[i];
        for (j = 0u; j < 8u; ++j) {
            if ((crc & 1u) != 0u) {
                crc = (crc >> 1u) ^ 0xEDB88320u;
            } else {
                crc >>= 1u;
            }
        }
    }

    return ~crc;
}

/*
 * 初始化代理。
 *
 * 初始化成功后，代理一定处于“已初始化”状态；区别只在于缓存是：
 * - 来自文件的有效配置（dirty=0）
 * - 或回退后的默认配置（dirty=1）
 */
int FlashConfigProxy_Init(FlashConfigProxy *p, const char *path)
{
    AppConfig loaded;

    if (p == NULL) {
        return FLASH_PROXY_ERR;
    }

    p->storage_path = (path != NULL) ? path : FLASH_STORAGE_DEFAULT_PATH;
    p->is_initialized = 1;
    p->is_cache_valid = 0;
    p->is_dirty = 0;

    if ((FlashStorage_ReadAll(p->storage_path, &loaded) == FLASH_PROXY_OK) &&
        (AppConfig_IsValid(&loaded) != 0)) {
        p->cache = loaded;
        p->is_cache_valid = 1;
        p->is_dirty = 0;
        return FLASH_PROXY_OK;
    }

    AppConfig_LoadDefault(&p->cache);
    p->is_cache_valid = 1;
    p->is_dirty = 1;
    return FLASH_PROXY_OK;
}

int FlashConfigProxy_Get(const FlashConfigProxy *p, AppConfig *out)
{
    if (p == NULL || out == NULL) {
        return FLASH_PROXY_ERR;
    }

    if (p->is_initialized == 0 || p->is_cache_valid == 0) {
        return FLASH_PROXY_ERR;
    }

    *out = p->cache;
    return FLASH_PROXY_OK;
}

/*
 * 整体写入接口。
 *
 * 这里故意不访问文件，符合代理规则：
 * 业务层的“写”只更新缓存，真正落盘只能通过 Commit 显式触发。
 */
int FlashConfigProxy_Set(FlashConfigProxy *p, const AppConfig *in)
{
    if (p == NULL || in == NULL) {
        return FLASH_PROXY_ERR;
    }

    if (p->is_initialized == 0 || p->is_cache_valid == 0) {
        return FLASH_PROXY_ERR;
    }

    p->cache = *in;
    p->cache.crc32 = AppConfig_CalcCrc32(&p->cache);
    p->is_dirty = 1;
    return FLASH_PROXY_OK;
}

int FlashConfigProxy_GetDeviceId(const FlashConfigProxy *p, uint8_t *out)
{
    if (p == NULL || out == NULL) {
        return FLASH_PROXY_ERR;
    }

    if (p->is_initialized == 0 || p->is_cache_valid == 0) {
        return FLASH_PROXY_ERR;
    }

    *out = p->cache.device_id;
    return FLASH_PROXY_OK;
}

int FlashConfigProxy_SetDeviceId(FlashConfigProxy *p, uint8_t v)
{
    if (p == NULL) {
        return FLASH_PROXY_ERR;
    }

    if (p->is_initialized == 0 || p->is_cache_valid == 0) {
        return FLASH_PROXY_ERR;
    }

    p->cache.device_id = v;
    p->cache.crc32 = AppConfig_CalcCrc32(&p->cache);
    p->is_dirty = 1;
    return FLASH_PROXY_OK;
}

int FlashConfigProxy_GetMode(const FlashConfigProxy *p, uint8_t *out)
{
    if (p == NULL || out == NULL) {
        return FLASH_PROXY_ERR;
    }

    if (p->is_initialized == 0 || p->is_cache_valid == 0) {
        return FLASH_PROXY_ERR;
    }

    *out = p->cache.mode;
    return FLASH_PROXY_OK;
}

int FlashConfigProxy_SetMode(FlashConfigProxy *p, uint8_t v)
{
    if (p == NULL) {
        return FLASH_PROXY_ERR;
    }

    if (p->is_initialized == 0 || p->is_cache_valid == 0) {
        return FLASH_PROXY_ERR;
    }

    p->cache.mode = v;
    p->cache.crc32 = AppConfig_CalcCrc32(&p->cache);
    p->is_dirty = 1;
    return FLASH_PROXY_OK;
}

/*
 * 显式提交。
 *
 * 这是唯一允许访问写文件逻辑的公开接口。
 * 提交成功后，说明 cache 与文件内容一致，因此 dirty 被清零。
 */
int FlashConfigProxy_Commit(FlashConfigProxy *p)
{
    if (p == NULL) {
        return FLASH_PROXY_ERR;
    }

    if (p->is_initialized == 0 || p->is_cache_valid == 0) {
        return FLASH_PROXY_ERR;
    }

    p->cache.crc32 = AppConfig_CalcCrc32(&p->cache);

    if (FlashStorage_WriteAll(p->storage_path, &p->cache) != FLASH_PROXY_OK) {
        return FLASH_PROXY_ERR;
    }

    p->is_dirty = 0;
    return FLASH_PROXY_OK;
}

/*
 * 重新加载。
 *
 * 该接口模拟“丢弃当前缓存，再从真实存储重新同步”的过程。
 * 如果底层文件无效，则仍然保持系统可运行：回退默认值并标记 dirty。
 */
int FlashConfigProxy_Reload(FlashConfigProxy *p)
{
    AppConfig loaded;

    if (p == NULL) {
        return FLASH_PROXY_ERR;
    }

    if (p->is_initialized == 0) {
        return FLASH_PROXY_ERR;
    }

    if ((FlashStorage_ReadAll(p->storage_path, &loaded) == FLASH_PROXY_OK) &&
        (AppConfig_IsValid(&loaded) != 0)) {
        p->cache = loaded;
        p->is_cache_valid = 1;
        p->is_dirty = 0;
        return FLASH_PROXY_OK;
    }

    AppConfig_LoadDefault(&p->cache);
    p->is_cache_valid = 1;
    p->is_dirty = 1;
    return FLASH_PROXY_OK;
}

int FlashConfigProxy_IsDirty(const FlashConfigProxy *p)
{
    if (p == NULL) {
        return FLASH_PROXY_ERR;
    }

    if (p->is_initialized == 0 || p->is_cache_valid == 0) {
        return FLASH_PROXY_ERR;
    }

    return p->is_dirty;
}
