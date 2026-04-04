#include "subject.h"

extern int printf(const char *format, ...);

void adc_subject_init(adc_subject_t *self)
{
    int i;

    if (self == 0) {
        return;
    }

    self->adc_value = 0;
    self->observer_count = 0;

    for (i = 0; i < MAX_OBSERVERS; ++i)
    {
        self->observers[i] = 0;
    }
}

int adc_subject_attach(adc_subject_t *self, observer_t *observer)
{
    int i;

    if (self == 0 || observer == 0) {
        return -1;
    }

    /* 禁止重复注册同一观察者。 */
    for (i = 0; i < self->observer_count; ++i)
    {
        if (self->observers[i] == observer) {
            return -1;
        }
    }

    if (self->observer_count >= MAX_OBSERVERS) {
        return -1;
    }

    self->observers[self->observer_count] = observer;
    self->observer_count += 1;
    return 0;
}

int adc_subject_detach(adc_subject_t *self, observer_t *observer)
{
    int i;

    if (self == 0 || observer == 0) {
        return -1;
    }

    for (i = 0; i < self->observer_count; ++i)
    {
        if (self->observers[i] == observer)
        {
            int j;
            for (j = i; j < self->observer_count - 1; ++j)
            {
                self->observers[j] = self->observers[j + 1];
            }
            self->observers[self->observer_count - 1] = 0;
            self->observer_count -= 1;
            return 0;
        }
    }

    return -1;
}

void adc_subject_notify(adc_subject_t *self)
{
    int i;

    if (self == 0) {
        return;
    }

    printf("[Subject] notify observers count=%d\n", self->observer_count);

    for (i = 0; i < self->observer_count; ++i)
    {
        observer_t *observer = self->observers[i];
        if (observer != 0 && observer->update != 0) {
            observer->update(observer, self->adc_value);
        }
    }
}

void adc_subject_set_value(adc_subject_t *self, int value)
{
    if (self == 0) {
        return;
    }

    self->adc_value = value;
    printf("[ADC] new sample = %d\n", value);
    adc_subject_notify(self);
}
