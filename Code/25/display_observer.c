#include "display_observer.h"

extern int printf(const char *format, ...);

/*
 * 显示观察者：仅负责将收到的 ADC 新值打印到“显示层”。
 */
static void display_observer_update(observer_t *base, int value)
{
    (void)base;
    printf("[Display] ADC value = %d\n", value);
}

void display_observer_init(display_observer_t *self)
{
    if (self == 0) {
        return;
    }

    self->base.update = display_observer_update;
}
