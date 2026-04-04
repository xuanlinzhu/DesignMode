#include "measurement_processor.h"

extern int printf(const char *format, ...);

static void fast_acquire(measurement_processor_t *self)
{
    self->raw_value = 1000;
    printf("[Step] acquire_data      -> raw = %d\n", self->raw_value);
}

static void fast_preprocess(measurement_processor_t *self)
{
    self->processed_value = self->raw_value - 5;
    printf("[Step] preprocess_data   -> processed = %d\n", self->processed_value);
}

static void fast_filter(measurement_processor_t *self)
{
    /* Fast 模式强调速度，滤波步骤实现为“直接透传”。 */
    self->processed_value = self->processed_value;
    printf("[Step] filter_data       -> processed = %d (pass-through)\n", self->processed_value);
}

static void fast_calibrate(measurement_processor_t *self)
{
    self->processed_value = self->processed_value + 2;
    printf("[Step] calibrate_data    -> processed = %d\n", self->processed_value);
}

static void fast_compute(measurement_processor_t *self)
{
    self->result_value = self->processed_value / 10;
    printf("[Step] compute_result    -> result = %d\n", self->result_value);
}

static void fast_output(measurement_processor_t *self)
{
    printf("[Step] output_result     -> final result = %d\n", self->result_value);
}

void fast_processor_init(fast_processor_t *self)
{
    if (self == 0) {
        return;
    }

    self->base.name = "FAST";
    self->base.raw_value = 0;
    self->base.processed_value = 0;
    self->base.result_value = 0;
    self->base.acquire_data = fast_acquire;
    self->base.preprocess_data = fast_preprocess;
    self->base.filter_data = fast_filter;
    self->base.calibrate_data = fast_calibrate;
    self->base.compute_result = fast_compute;
    self->base.output_result = fast_output;
}
