#include <stdio.h>
#include <string.h>
#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "flash_log_storage.h"
#include "log_viewer.h"
#include "ram_log_storage.h"

static void init_console_utf8(void)
{
    (void)setlocale(LC_ALL, ".UTF-8");

#ifdef _WIN32
    (void)SetConsoleOutputCP(65001u);
    (void)SetConsoleCP(65001u);
#endif
}

/*
 * 构造一个日志项，统一拷贝 message，避免外部直接写容器内部结构。
 */
static log_entry_t make_log(int id, int level, const char *msg)
{
    log_entry_t entry;

    entry.id = id;
    entry.level = level;
    strncpy(entry.message, msg, sizeof(entry.message) - 1);
    entry.message[sizeof(entry.message) - 1] = '\0';
    return entry;
}

int main(void)
{
    ram_log_storage_t ram_storage;
    flash_log_storage_t flash_storage;
    log_iterator_t *it;
    int i;

    init_console_utf8();

    printf("==============================\n");
    printf("[Demo] Iterator 模式\n");
    printf("==============================\n");

    /* 场景1：初始化并写入日志（RAM / Flash 各至少 5 条）。 */
    ram_log_init(&ram_storage);
    flash_log_init(&flash_storage);

    for (i = 1; i <= 5; i++) {
        char msg[64];
        log_entry_t entry;

        (void)snprintf(msg, sizeof(msg), "ram log %d", i);
        entry = make_log(i, (i % 3) + 1, msg);
        ram_log_add(&ram_storage, &entry);
    }

    for (i = 1; i <= 5; i++) {
        char msg[64];
        log_entry_t entry;

        (void)snprintf(msg, sizeof(msg), "flash log %d", i);
        entry = make_log(100 + i, (i % 2) + 1, msg);
        flash_log_add(&flash_storage, &entry);
    }

    printf("\n[Scenario2] iterate RAM logs\n");
    it = ram_log_create_iterator(&ram_storage);
    log_viewer_show("RAM", it);

    printf("\n[Scenario3] iterate FLASH logs\n");
    it = flash_log_create_iterator(&flash_storage);
    log_viewer_show("FLASH", it);

    /*
     * 场景4：验证 RAM 环形覆盖。
     * 再追加超过容量的数据后，遍历结果应只保留最新 RAM_LOG_CAPACITY 条。
     */
    printf("\n[Scenario4] RAM overflow then keep latest logs\n");
    for (i = 6; i <= 12; i++) {
        char msg[64];
        log_entry_t entry;

        (void)snprintf(msg, sizeof(msg), "ram log %d", i);
        entry = make_log(i, (i % 4) + 1, msg);
        ram_log_add(&ram_storage, &entry);
    }

    it = ram_log_create_iterator(&ram_storage);
    log_viewer_show("RAM", it);

    printf("\n==============================\n");
    printf("[Demo] done\n");
    return 0;
}
