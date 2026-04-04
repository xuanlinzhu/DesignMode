#include <stdio.h>
#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "debug_caretaker.h"
#include "default_caretaker.h"
#include "instrument_param.h"

static void init_console_utf8(void)
{
    (void)setlocale(LC_ALL, ".UTF-8");

#ifdef _WIN32
    (void)SetConsoleOutputCP(65001u);
    (void)SetConsoleCP(65001u);
#endif
}

int main(void)
{
    instrument_param_t param;
    debug_param_caretaker_t debug_keeper;
    default_param_caretaker_t default_keeper;

    init_console_utf8();

    printf("==============================\n");
    printf("[Demo] Memento 模式\n");
    printf("==============================\n");

    instrument_param_init(&param, 10, 0, 100, 1);
    debug_caretaker_init(&debug_keeper);
    default_caretaker_init(&default_keeper);

    /* 初始化后保存长期有效的默认参数快照。 */
    default_caretaker_save(&default_keeper, instrument_param_create_memento(&param));

    printf("\n[场景1] 进入调试模式前保存快照，退出调试后恢复\n");
    printf("[User] enter debug mode\n");
    debug_caretaker_save(&debug_keeper, instrument_param_create_memento(&param));

    instrument_param_set_gain(&param, 20);
    instrument_param_set_threshold(&param, 150);
    instrument_param_set_mode(&param, 2);
    instrument_param_print(&param);

    printf("[User] exit debug mode and restore\n");
    if (debug_caretaker_has_snapshot(&debug_keeper)) {
        printf("[DebugCaretaker] restore debug snapshot\n");
        instrument_param_restore_memento(&param, debug_caretaker_get(&debug_keeper));
        debug_caretaker_clear(&debug_keeper);
    } else {
        printf("[DebugCaretaker] no debug snapshot\n");
    }
    instrument_param_print(&param);

    printf("\n[场景2] 恢复系统默认参数\n");
    instrument_param_set_gain(&param, 30);
    instrument_param_set_offset(&param, 8);
    instrument_param_set_mode(&param, 3);
    instrument_param_print(&param);

    printf("[User] restore default param\n");
    if (default_caretaker_has_snapshot(&default_keeper)) {
        printf("[DefaultCaretaker] restore default snapshot\n");
        instrument_param_restore_memento(&param, default_caretaker_get(&default_keeper));
    } else {
        printf("[DefaultCaretaker] no default snapshot\n");
    }
    instrument_param_print(&param);

    printf("\n==============================\n");
    printf("[Demo] done\n");
    return 0;
}
