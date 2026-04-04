#include <stdio.h>
#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "mediator.h"
#include "modules.h"

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
    instrument_mediator_t mediator;
    alarm_module_t alarm;
    debug_module_t debug;
    run_module_t run;
    display_module_t display;
    buzzer_module_t buzzer;
    led_module_t led;

    init_console_utf8();

    mediator_init(&mediator);

    alarm_module_init(&alarm, &mediator);
    debug_module_init(&debug, &mediator);
    run_module_init(&run, &mediator);
    display_module_init(&display, &mediator);
    buzzer_module_init(&buzzer, &mediator);
    led_module_init(&led, &mediator);

    mediator_bind_modules(&mediator, &alarm, &debug, &run, &display, &buzzer, &led);

    printf("==============================\n");
    printf("[Demo] Mediator 模式\n");
    printf("==============================\n");

    printf("\n[场景1] 正常启动运行\n");
    run_module_request_start(&run);
    mediator_dump_state(&mediator);

    printf("\n[场景2] 运行中触发报警\n");
    alarm_module_trigger(&alarm);
    mediator_dump_state(&mediator);

    printf("\n[场景3] 解除报警后恢复待机\n");
    alarm_module_clear(&alarm);
    mediator_dump_state(&mediator);

    printf("\n[场景4] 进入调试模式后拒绝运行\n");
    debug_module_enter(&debug);
    run_module_request_start(&run);
    mediator_dump_state(&mediator);

    printf("\n[场景5] 手动触发报警（联动与普通报警一致）\n");
    alarm_module_manual_trigger(&alarm);
    mediator_dump_state(&mediator);

    /* 为场景6恢复运行创造条件：先解除报警，再退出调试。 */
    alarm_module_clear(&alarm);

    printf("\n[场景6] 退出调试模式后可重新运行\n");
    debug_module_exit(&debug);
    run_module_request_start(&run);
    mediator_dump_state(&mediator);

    printf("\n==============================\n");
    printf("[Demo] done\n");
    return 0;
}
