#include <stdio.h>
#include <string.h>
#include "job.h"

void job_manager_init(JobManager *manager) { memset(manager, 0, sizeof(*manager)); }
int job_name_exists(const JobManager *manager, const char *name) { for (int i = 0; i < manager->count; i++) if (!strcmp(manager->jobs[i].name, name)) return 1; return 0; }

static void clear_client_claim(BankerSystem *banker, int client) {
    for (int r = 0; r < banker->num_resources; r++) banker->clients[client].maximum[r] = banker->clients[client].need[r] = 0;
}

int submit_job(JobManager *manager, BankerSystem *banker, MemoryBlock blocks[], int num_blocks,
               Process processes[], int *process_count, const Job *input, AllocationStrategy strategy,
               char *message, int message_size) {
    if (!input->name[0]) { snprintf(message, message_size, "Job name is required."); return 0; }
    if (job_name_exists(manager, input->name)) { snprintf(message, message_size, "Job name '%s' already exists.", input->name); return 0; }
    if (manager->count >= MAX_JOBS || *process_count >= MAX_PROCESSES) { snprintf(message, message_size, "Job capacity has been reached."); return 0; }
    if (!num_blocks) { snprintf(message, message_size, "Initialize storage before submitting a job."); return 0; }
    if (banker->num_resources <= 0 || banker->num_clients <= 0) { snprintf(message, message_size, "Initialize the Banker system before submitting a job."); return 0; }
    if (input->cpu_burst <= 0 || input->priority < 0 || input->arrival_time < 0 || input->memory_size <= 0) { snprintf(message, message_size, "Burst and memory must be positive; arrival and priority cannot be negative."); return 0; }

    Job *job = &manager->jobs[manager->count];
    *job = *input; job->client_id = manager->count; job->status = JOB_QUEUED;
    for (int r = 0; r < banker->num_resources; r++) {
        if (job->claim[r] < 0 || job->request[r] < 0 || job->request[r] > job->claim[r]) { snprintf(message, message_size, "Each resource request must be between 0 and its declared maximum claim."); return 0; }
        banker->clients[job->client_id].maximum[r] = banker->clients[job->client_id].need[r] = job->claim[r];
    }
    manager->count++;
    if (!request_resources(banker, job->client_id, job->request)) {
        job->status = JOB_DENIED; manager->denied_count++;
        snprintf(message, message_size, "Job '%s' queued: resource request was denied (unsafe or unavailable).", job->name);
        return 0;
    }
    if (!allocate_memory(blocks, num_blocks, job->memory_size, job->name, strategy)) {
        release_resources(banker, job->client_id, job->request);
        clear_client_claim(banker, job->client_id);
        job->status = JOB_DENIED; manager->denied_count++;
        snprintf(message, message_size, "Job '%s' denied: memory allocation failed; resources were released.", job->name);
        return 0;
    }
    Process *process = &processes[(*process_count)++];
    memset(process, 0, sizeof(*process)); process->pid = job->client_id;
    snprintf(process->name, sizeof(process->name), "%s", job->name);
    process->arrival_time = job->arrival_time; process->burst_time = job->cpu_burst; process->priority = job->priority;
    job->memory_allocated = job->scheduled = 1; job->status = JOB_GRANTED; manager->granted_count++;
    snprintf(message, message_size, "Job '%s' granted: resources, memory, and CPU queue registered.", job->name);
    return 1;
}

int complete_job(JobManager *manager, BankerSystem *banker, MemoryBlock blocks[], int num_blocks,
                 Process processes[], int *process_count, const char *name, char *message, int message_size) {
    for (int j = 0; j < manager->count; j++) if (!strcmp(manager->jobs[j].name, name)) {
        Job *job = &manager->jobs[j];
        if (job->status != JOB_GRANTED) { snprintf(message, message_size, "Only granted jobs can be completed."); return 0; }
        release_resources(banker, job->client_id, job->request); deallocate_memory(blocks, num_blocks, job->name);
        for (int i = 0; i < *process_count; i++) if (!strcmp(processes[i].name, name)) { for (int k = i; k < *process_count - 1; k++) processes[k] = processes[k + 1]; (*process_count)--; break; }
        job->memory_allocated = job->scheduled = 0; job->status = JOB_COMPLETED;
        snprintf(message, message_size, "Job '%s' completed: resources and memory released.", name); return 1;
    }
    snprintf(message, message_size, "No job named '%s' exists.", name); return 0;
}

void copy_schedule_metrics(JobManager *manager, const Process processes[], int process_count) {
    for (int i = 0; i < process_count; i++) for (int j = 0; j < manager->count; j++)
        if (!strcmp(processes[i].name, manager->jobs[j].name)) { manager->jobs[j].waiting_time = processes[i].waiting_time; manager->jobs[j].turnaround_time = processes[i].turnaround_time; manager->jobs[j].has_metrics = 1; }
}
