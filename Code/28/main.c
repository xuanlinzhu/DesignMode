#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "measurement_processor.h"

extern int printf(const char *format, ...);

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
    normal_processor_t normal;
    fast_processor_t fast;
    precise_processor_t precise;

    init_console_utf8();

    normal_processor_init(&normal);
    fast_processor_init(&fast);
    precise_processor_init(&precise);

    printf("========================================\n");
    printf("[场景1] Normal 模式\n");
    measurement_process(&normal.base);

    printf("========================================\n");
    printf("[场景2] Fast 模式\n");
    measurement_process(&fast.base);

    printf("========================================\n");
    printf("[场景3] Precise 模式\n");
    measurement_process(&precise.base);

    printf("========================================\n");
    return 0;
}
