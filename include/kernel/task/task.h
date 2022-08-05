
#ifndef TASK_H
#define TASK_H

#define THREAD_SIZE 4096
#define NR_TASKS    64
#define TASK_FIRST  tasks[0]
#define TASK_LAST   tasks[NR_TASKS - 1]

#define TASK_RUNNING    0

#include <types.h>

typedef struct {
    uint64_t x19;
    uint64_t x20;
    uint64_t x21;
    uint64_t x22;
    uint64_t x23;
    uint64_t x24;
    uint64_t x25;
    uint64_t x26;
    uint64_t x27;
    uint64_t x28;
    uint64_t fp;
    uint64_t sp;
    uint64_t pc;
} cpu_ctx_t;

typedef struct {
    cpu_ctx_t ctx;
    uint64_t state;
    uint64_t counter;
    uint64_t priority;
    uint64_t preempt_count;
} task_t;

extern task_t *current_task;
extern task_t *tasks[NR_TASKS];

#endif