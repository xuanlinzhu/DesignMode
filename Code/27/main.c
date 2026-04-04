#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "filter_strategy.h"

extern int printf(const char *format, ...);

static void init_console_utf8(void)
{
    (void)setlocale(LC_ALL, ".UTF-8");

#ifdef _WIN32
    (void)SetConsoleOutputCP(65001u);
    (void)SetConsoleCP(65001u);
#endif
}

static void run_samples(filter_context_t *ctx, const int *samples, int count)
{
    int i;

    if (ctx == 0 || samples == 0 || count <= 0) {
        return;
    }

    for (i = 0; i < count; ++i)
    {
        int out = filter_process(ctx, samples[i]);
        printf("[Result] strategy=%s input=%d final_output=%d\n", ctx->strategy->name, samples[i], out);
    }
}

int main(void)
{
    int samples[] = {100, 102, 101, 250, 103, 102};
    filter_context_t ctx;
    raw_strategy_t raw;
    average_strategy_t avg;
    clamp_strategy_t clamp;

    init_console_utf8();

    raw_strategy_init(&raw);
    average_strategy_init(&avg);
    clamp_strategy_init(&clamp, 20);

    printf("========================================\n");
    printf("[场景1] RAW 策略处理\n");
    filter_context_init(&ctx, &raw.base);
    run_samples(&ctx, samples, 6);

    printf("========================================\n");
    printf("[场景2] 切换到 AVG(window=3) 策略\n");
    filter_set_strategy(&ctx, &avg.base);
    run_samples(&ctx, samples, 6);

    printf("========================================\n");
    printf("[场景3] 切换到 CLAMP(threshold=20) 策略\n");
    filter_set_strategy(&ctx, &clamp.base);
    run_samples(&ctx, samples, 6);

    printf("========================================\n");
    return 0;
}
