#ifndef SUBJECT_H
#define SUBJECT_H

#include "observer.h"

#define MAX_OBSERVERS 8

/*
 * ADC Subject：单一数据生产者。
 *
 * 该结构只保存 ADC 当前值和观察者列表，不包含任何业务策略：
 * - 不关心阈值判断
 * - 不关心日志格式
 * - 不关心通信上传
 *
 * 它只负责“发布通知”。
 */
typedef struct adc_subject
{
    int adc_value;
    observer_t *observers[MAX_OBSERVERS];
    int observer_count;
} adc_subject_t;

void adc_subject_init(adc_subject_t *self);
int adc_subject_attach(adc_subject_t *self, observer_t *observer);
int adc_subject_detach(adc_subject_t *self, observer_t *observer);
void adc_subject_set_value(adc_subject_t *self, int value);
void adc_subject_notify(adc_subject_t *self);

#endif
