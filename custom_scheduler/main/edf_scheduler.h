#ifndef LAB06_EDF_SCHEDULER_H
#define LAB06_EDF_SCHEDULER_H
#include <stdint.h>

typedef struct {
    uint32_t period_ticks;
    uint32_t deadline;
    uint32_t priority;
    void (*tick_function)(void);
} task_t;


#endif //LAB06_EDF_SCHEDULER_H
