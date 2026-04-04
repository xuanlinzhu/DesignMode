#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "instrument_states.h"

extern int printf(const char *format, ...);

static void init_console_utf8(void)
{
    (void)setlocale(LC_ALL, ".UTF-8");

#ifdef _WIN32
    (void)SetConsoleOutputCP(65001u);
    (void)SetConsoleCP(65001u);
#endif
}

static void run_event_sequence(const char *title, const event_t *events, int count)
{
    int i;
    state_context_t ctx;

    if (title == 0 || events == 0 || count <= 0) {
        return;
    }

    printf("========================================\n");
    printf("%s\n", title);

    /* 每个场景都从 IDLE 重新初始化，便于验证预期状态迁移。 */
    state_machine_init(&ctx, &idle_state);

    for (i = 0; i < count; ++i)
    {
        state_machine_dispatch(&ctx, events[i]);
    }

    printf("[Scenario End] current state = %s\n", ctx.current_state->name);
}

int main(void)
{
    event_t scenario1[] = {EVENT_START, EVENT_STOP};
    event_t scenario2[] = {EVENT_START, EVENT_ALARM_ON, EVENT_ALARM_CLEAR};
    event_t scenario3[] = {EVENT_ENTER_DEBUG, EVENT_EXIT_DEBUG};
    event_t scenario4[] = {EVENT_ENTER_DEBUG, EVENT_START, EVENT_EXIT_DEBUG};
    event_t scenario5[] = {EVENT_ALARM_ON, EVENT_START, EVENT_ALARM_CLEAR};

    init_console_utf8();

    run_event_sequence("[场景1] 正常启动与停止：IDLE -> RUNNING -> IDLE", scenario1, 2);
    run_event_sequence("[场景2] 运行中触发报警：IDLE -> RUNNING -> ALARM -> IDLE", scenario2, 3);
    run_event_sequence("[场景3] 进入调试再退出：IDLE -> DEBUG -> IDLE", scenario3, 2);
    run_event_sequence("[场景4] DEBUG 下尝试 START：必须拒绝并保持 DEBUG", scenario4, 3);
    run_event_sequence("[场景5] ALARM 下尝试 START：必须拒绝并保持 ALARM", scenario5, 3);

    printf("========================================\n");
    return 0;
}
