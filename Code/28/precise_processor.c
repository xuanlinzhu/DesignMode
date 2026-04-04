#include "measurement_processor.h"

extern int printf(const char *format, ...);

static void precise_acquire(measurement_processor_t *self)
{
    self->raw_value = 1000;
    printf("[Step] acquire_data      -> raw = %d\n", self->raw_value);
}

static void precise_preprocess(measurement_processor_t *self)
{
    self->processed_value = self->raw_value - 12;
    printf("[Step] preprocess_data   -> processed = %d\n", self->processed_value);
}

static void precise_filter(measurement_processor_t *self)
{
    self->processed_value = self->processed_value - 4;
    printf("[Step] filter_data       -> processed = %d\n", self->processed_value);
}

static void precise_calibrate(measurement_processor_t *self)
{
    self->processed_value = self->processed_value + 9;
    printf("[Step] calibrate_data    -> processed = %d\n", self->processed_value);
}

static void precise_compute(measurement_processor_t *self)
{
    /* 模拟更精细的工程量换算（整数表示）：扩大后再回缩，保留细粒度修正。 */
    self->result_value = (self->processed_value * 12) / 100;
    printf("[Step] compute_result    -> result = %d\n", self->result_value);
}

static void precise_output(measurement_processor_t *self)
{
    printf("[Step] output_result     -> final result = %d\n", self->result_value);
}

void precise_processor_init(precise_processor_t *self)
{
    if (self == 0) {
        return;
    }

    self->base.name = "PRECISE";
    self->base.raw_value = 0;
    self->base.processed_value = 0;
    self->base.result_value = 0;
    self->base.acquire_data = precise_acquire;
    self->base.preprocess_data = precise_preprocess;
    self->base.filter_data = precise_filter;
    self->base.calibrate_data = precise_calibrate;
    self->base.compute_result = precise_compute;
    self->base.output_result = precise_output;
}
