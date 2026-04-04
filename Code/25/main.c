#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "alarm_observer.h"
#include "comm_observer.h"
#include "display_observer.h"
#include "logger_observer.h"
#include "subject.h"

extern int printf(const char *format, ...);

static void init_console_utf8(void)
{
    (void)setlocale(LC_ALL, ".UTF-8");

#ifdef _WIN32
    (void)SetConsoleOutputCP(65001u);
    (void)SetConsoleCP(65001u);
#endif
}

static void publish_sequence(adc_subject_t *adc, const int *samples, int count)
{
    int i;

    if (adc == 0 || samples == 0 || count <= 0) {
        return;
    }

    for (i = 0; i < count; ++i)
    {
        printf("------------------------------\n");
        adc_subject_set_value(adc, samples[i]);
    }
}

int main(void)
{
    adc_subject_t adc;
    display_observer_t display;
    alarm_observer_t alarm;
    logger_observer_t logger;
    comm_observer_t comm;
    int samples_before_detach[] = {120, 350, 820, 1023};
    int samples_after_detach[] = {600};

    init_console_utf8();

    /*
     * 场景 1：创建一个 Subject，并注册四个观察者。
     * 这体现“一个生产者，多个消费者”的拓扑结构。
     */
    adc_subject_init(&adc);
    display_observer_init(&display);
    alarm_observer_init(&alarm, 800);
    logger_observer_init(&logger);
    comm_observer_init(&comm);

    adc_subject_attach(&adc, &display.base);
    adc_subject_attach(&adc, &alarm.base);
    adc_subject_attach(&adc, &logger.base);
    adc_subject_attach(&adc, &comm.base);

    printf("========================================\n");
    printf("[场景1+2] 四个观察者都已注册，ADC 连续发布 120/350/820/1023\n");
    publish_sequence(&adc, samples_before_detach, 4);

    /*
     * 场景 3：取消 Logger 订阅。
     * 后续发布中，Logger 不应再收到任何 update 通知。
     */
    printf("========================================\n");
    printf("[场景3] detach LoggerObserver\n");
    if (adc_subject_detach(&adc, &logger.base) == 0) {
        printf("[Subject] detach LoggerObserver success\n");
    }
    else {
        printf("[Subject] detach LoggerObserver failed\n");
    }

    printf("[场景3] 继续发布 600，验证 Logger 不再输出\n");
    publish_sequence(&adc, samples_after_detach, 1);
    printf("========================================\n");
    return 0;
}
