#include "measurement_processor.h"

extern int printf(const char *format, ...);

/*
 * 模板方法：固定测量流程骨架。
 *
 * 严格顺序：
 * acquire -> preprocess -> filter -> calibrate -> compute -> output
 */
void measurement_process(measurement_processor_t *self)
{
    if (self == 0) {
        return;
    }

    if (self->acquire_data == 0 || self->preprocess_data == 0 || self->filter_data == 0 ||
        self->calibrate_data == 0 || self->compute_result == 0 || self->output_result == 0) {
        return;
    }

    printf("[Process] mode = %s\n", self->name);
    self->acquire_data(self);
    self->preprocess_data(self);
    self->filter_data(self);
    self->calibrate_data(self);
    self->compute_result(self);
    self->output_result(self);
}
