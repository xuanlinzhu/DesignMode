#ifndef PARAM_MEMENTO_H
#define PARAM_MEMENTO_H

/* 备忘录对象：完整保存参数快照。 */
typedef struct
{
    int gain;
    int offset;
    int threshold;
    int mode;
} param_memento_t;

#endif
