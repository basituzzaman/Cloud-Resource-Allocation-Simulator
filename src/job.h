#ifndef JOB_H
#define JOB_H

#include "banker.h"
#include "scheduler.h"
#include "storage.h"

#define MAX_JOBS MAX_CLIENTS

typedef enum { JOB_QUEUED, JOB_GRANTED, JOB_DENIED, JOB_COMPLETED } JobStatus;

typedef struct {
    char name[MAX_NAME];
    int cpu_burst, priority, arrival_time, memory_size;
    int claim[MAX_RESOURCES], request[MAX_RESOURCES];
    int client_id, waiting_time, turnaround_time;
    int memory_allocated, scheduled, has_metrics;
    JobStatus status;
} Job;

typedef struct { Job jobs[MAX_JOBS]; int count, granted_count, denied_count; } JobManager;

void job_manager_init(JobManager *manager);
int job_name_exists(const JobManager *manager, const char *name);
int submit_job(JobManager *manager, BankerSystem *banker, MemoryBlock blocks[], int num_blocks,
               Process processes[], int *process_count, const Job *input, AllocationStrategy strategy,
               char *message, int message_size);
int complete_job(JobManager *manager, BankerSystem *banker, MemoryBlock blocks[], int num_blocks,
                 Process processes[], int *process_count, const char *name, char *message, int message_size);
void copy_schedule_metrics(JobManager *manager, const Process processes[], int process_count);

#endif
