#include "flash_config_proxy.h"

extern int printf(const char *format, ...);
extern int puts(const char *str);

/*
 * 教学输出函数。
 *
 * 这里把当前缓存中的关键字段完整打印出来，便于观察：
 * - 初始化时读到的是默认值还是文件值
 * - 修改并提交后，哪些字段已经变化
 * - Reload 后缓存是否与文件重新同步
 */
static void PrintConfig(const AppConfig *cfg, const char *title)
{
    if (cfg == 0 || title == 0) {
        return;
    }

    printf("%s\n", title);
    printf("  version=0x%08X\n", cfg->version);
    printf("  device_id=%u\n", (unsigned int)cfg->device_id);
    printf("  mode=%u\n", (unsigned int)cfg->mode);
    printf("  sample_interval_ms=%u\n", (unsigned int)cfg->sample_interval_ms);
    printf("  baudrate=%u\n", (unsigned int)cfg->baudrate);
    printf("  enable_log=%u\n", (unsigned int)cfg->enable_log);
    printf("  crc32=0x%08X\n", cfg->crc32);
}

int main(void)
{
    FlashConfigProxy proxy;
    AppConfig cfg;
    int ret;

    /*
     * 第 1 步：初始化代理。
     * 若文件不存在或内容非法，代理会自动回退到默认配置，
     * 但初始化本身仍然成功。
     */
    ret = FlashConfigProxy_Init(&proxy, "flash_storage.bin");
    if (ret != FLASH_PROXY_OK) {
        puts("Init failed");
        return 1;
    }

    /* 第 2 步：读取缓存配置，验证业务层读操作不触发文件访问。 */
    ret = FlashConfigProxy_Get(&proxy, &cfg);
    if (ret != FLASH_PROXY_OK) {
        puts("Get failed");
        return 1;
    }
    PrintConfig(&cfg, "[Init] Cache config:");

    /*
     * 第 3 步：修改两个字段。
     * 这里只改 cache，不直接写文件，因此修改后 dirty 应为 1。
     */
    ret = FlashConfigProxy_SetDeviceId(&proxy, 7u);
    if (ret != FLASH_PROXY_OK) {
        puts("SetDeviceId failed");
        return 1;
    }
    ret = FlashConfigProxy_SetMode(&proxy, 2u);
    if (ret != FLASH_PROXY_OK) {
        puts("SetMode failed");
        return 1;
    }

    printf("[After Set] dirty=%d\n", FlashConfigProxy_IsDirty(&proxy));

    /* 第 4 步：显式提交，提交后文件与缓存同步，dirty 应清零。 */
    ret = FlashConfigProxy_Commit(&proxy);
    if (ret != FLASH_PROXY_OK) {
        puts("Commit failed");
        return 1;
    }
    printf("[After Commit] dirty=%d\n", FlashConfigProxy_IsDirty(&proxy));

    /* 第 5 步：重新加载，验证文件中的持久化结果是否能重新读回 cache。 */
    ret = FlashConfigProxy_Reload(&proxy);
    if (ret != FLASH_PROXY_OK) {
        puts("Reload failed");
        return 1;
    }

    ret = FlashConfigProxy_Get(&proxy, &cfg);
    if (ret != FLASH_PROXY_OK) {
        puts("Get after reload failed");
        return 1;
    }
    PrintConfig(&cfg, "[Reload] Cache config:");

    return 0;
}
