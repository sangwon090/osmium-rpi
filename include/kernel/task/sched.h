#include <types.h>
#include <task/task.h>

#ifndef SCHEDULER_H
#define SCHEDULER_H

#define THREAD_CPU_CONTEXT			0 		// offset of cpu_context in task_struct 

void _schedule();
void schedule();
void scheduler_tick();
void switch_to(task_t *next);
int32_t process_create(uint64_t fn, uint64_t args);
void ret_from_fork();

extern void cpu_switch_to(task_t *prev, task_t *next);

#define INIT_TASK { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 1, 0 }

#endif