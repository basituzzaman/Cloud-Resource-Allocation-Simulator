#ifndef SCHEDULER_H
#define SCHEDULER_H

#define MAX_PROCESSES 50
#define MAX_NAME 20

typedef struct {
    int pid;
    char name[MAX_NAME];
    int arrival_time, burst_time, remaining_time, priority;
    int completion_time, waiting_time, turnaround_time, is_completed;
} Process;

typedef enum { FCFS, SJF, ROUND_ROBIN, PRIORITY } SchedulingAlgorithm;

void schedule_fcfs(Process processes[], int n);
void schedule_sjf(Process processes[], int n);
void schedule_round_robin(Process processes[], int n, int quantum);
void schedule_priority(Process processes[], int n);

#endif
