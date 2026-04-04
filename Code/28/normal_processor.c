#include "measurement_processor.h"

extern int printf(const char *format, ...);

static void normal_acquire(measurement_processor_t *self)
{
    self->raw_value = 1000;
    printf("[Step] acquire_data      -> raw = %d\n", self->raw_value);
}

static void normal_preprocess(measurement_processor_t *self)
{
    self->processed_value = self->raw_value - 10;
    printf("[Step] preprocess_data   -> processed = %d\n", self->processed_value);
}

static void normal_filter(measurement_processor_t *self)
{
    self->processed_value = self->processed_value - 2;
    printf("[Step] filter_data       -> processed = %d\n", self->processed_value);
}

static void normal_calibrate(measurement_processor_t *self)
{
    self->processed_value = self->processed_value + 5;
    printf("[Step] calibrate_data    -> processed = %d\n", self->processed_value);
}

static void normal_compute(measurement_processor_t *self)
{
    self->result_value = self->processed_value / 10;
    printf("[Step] compute_result    -> result = %d\n", self->result_value);
}

static void normal_output(measurement_processor_t *self)
{
    printf("[Step] output_result     -> final result = %d\n", self->result_value);
}

void normal_processor_init(normal_processor_t *self)
{
    if (self == 0) {
        return;
    }

    self->base.name = "NORMAL";
    self->base.raw_value = 0;
    self->base.processed_value = 0;
    self->base.result_value = 0;
    self->base.acquire_data = normal_acquire;
    self->base.preprocess_data = normal_preprocess;
    self->base.filter_data = normal_filter;
    self->base.calibrate_data = normal_calibrate;
    self->base.compute_result = normal_compute;
    self->base.output_result = normal_output;
}
