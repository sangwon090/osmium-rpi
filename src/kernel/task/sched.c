#include <mm.h>
#include <task/sched.h>

static task_t init_task = INIT_TASK;
task_t *current = &(init_task);
task_t *tasks[NR_TASKS] = { &(init_task), };
int32_t nr_tasks = 1;

void preempt_enable()
{
    current->preempt_count --;
}

void preempt_disable()
{
    current->preempt_count ++;
}

void _schedule()
{
    preempt_disable();

    int32_t next, c;
    task_t *p;

    while(1)
    {
        c = -1;
        next = 0;

        for(int i=0; i< NR_TASKS; i++)
        {
            p = tasks[i];

            if(p && p->state == TASK_RUNNING && p->counter > c)
            {
                c = p->counter;
                next = i;
            }
        }

        if(c)
        {
            break;
        }

        for(int i=0; i<NR_TASKS; i++)
        {
            p = tasks[i];

            if(p)
            {
                p->counter = (p->counter >> 1) + p->priority;
            }
        }
    }

    switch_to(tasks[next]);
    preempt_enable();
}

void schedule()
{
    current->counter = 0;
    _schedule();
}

void switch_to(task_t *next)
{
    if(current == next) return;

    task_t *prev = current;
    current = next;
    cpu_switch_to(prev, next);
}

void schedule_tail(void)
{
	preempt_enable();
}

int32_t process_create(uint64_t fn, uint64_t args)
{
    preempt_disable();
    task_t *p;

    p = (task_t*) mm_get_free_page();
    //uart_printf("p = %X_%X\r\n", (uint64_t) p >> 32, (uint64_t) p);
    //while(1);

    if(!p) return 1;

    p->priority = current->priority;
    p->state = TASK_RUNNING;
    p->counter = p->priority;
    p->preempt_count = 1;

    p->ctx.x19 = fn;
    p->ctx.x20 = args;
    p->ctx.pc = (uint64_t) ret_from_fork;
    p->ctx.sp = (uint64_t) p + THREAD_SIZE;

    int32_t pid = nr_tasks ++;
    tasks[pid] = p;
    preempt_enable();

    return 0;
}

void scheduler_tick()
{
    -- current->counter;

    if(current->counter > 0 || current->preempt_count > 0)
    {
        return;
    }

    current->counter = 0;
    irq_enable();
    _schedule();
    irq_disable();
}