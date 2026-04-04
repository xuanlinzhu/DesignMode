#ifndef MEASUREMENT_PROCESSOR_H
#define MEASUREMENT_PROCESSOR_H

typedef struct measurement_processor measurement_processor_t;

struct measurement_processor
{
    const char *name;

    int raw_value;
    int processed_value;
    int result_value;

    void (*acquire_data)(measurement_processor_t *self);
    void (*preprocess_data)(measurement_processor_t *self);
    void (*filter_data)(measurement_processor_t *self);
    void (*calibrate_data)(measurement_processor_t *self);
    void (*compute_result)(measurement_processor_t *self);
    void (*output_result)(measurement_processor_t *self);
};

typedef struct
{
    measurement_processor_t base;
} normal_processor_t;

typedef struct
{
    measurement_processor_t base;
} fast_processor_t;

typedef struct
{
    measurement_processor_t base;
} precise_processor_t;

void measurement_process(measurement_processor_t *self);

void normal_processor_init(normal_processor_t *self);
void fast_processor_init(fast_processor_t *self);
void precise_processor_init(precise_processor_t *self);

#endif
