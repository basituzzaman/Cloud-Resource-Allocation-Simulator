#include "scheduler.h"

static void reset(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
        p[i].completion_time = p[i].waiting_time = p[i].turnaround_time = 0;
        p[i].is_completed = 0;
    }
}

static void finish(Process *p, int time) {
    p->completion_time = time;
    p->turnaround_time = time - p->arrival_time;
    p->waiting_time = p->turnaround_time - p->burst_time;
    p->is_completed = 1;
}

void schedule_fcfs(Process p[], int n) {
    reset(p, n);
    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival_time) time = p[i].arrival_time;
        time += p[i].burst_time;
        finish(&p[i], time);
    }
}

void schedule_sjf(Process p[], int n) {
    reset(p, n);
    int time = 0, completed = 0;
    while (completed < n) {
        int pick = -1;
        for (int i = 0; i < n; i++)
            if (!p[i].is_completed && p[i].arrival_time <= time &&
                (pick < 0 || p[i].burst_time < p[pick].burst_time ||
                 (p[i].burst_time == p[pick].burst_time && p[i].arrival_time < p[pick].arrival_time))) pick = i;
        if (pick < 0) { time++; continue; }
        time += p[pick].burst_time;
        finish(&p[pick], time);
        completed++;
    }
}

void schedule_priority(Process p[], int n) {
    reset(p, n);
    int time = 0, completed = 0;
    while (completed < n) {
        int pick = -1;
        for (int i = 0; i < n; i++)
            if (!p[i].is_completed && p[i].arrival_time <= time &&
                (pick < 0 || p[i].priority < p[pick].priority)) pick = i;
        if (pick < 0) { time++; continue; }
        time += p[pick].burst_time;
        finish(&p[pick], time);
        completed++;
    }
}

void schedule_round_robin(Process p[], int n, int quantum) {
    reset(p, n);
    int time = 0, completed = 0;
    while (completed < n) {
        int ran = 0;
        for (int i = 0; i < n; i++) {
            if (p[i].is_completed || p[i].arrival_time > time) continue;
            int slice = p[i].remaining_time < quantum ? p[i].remaining_time : quantum;
            time += slice;
            p[i].remaining_time -= slice;
            ran = 1;
            if (!p[i].remaining_time) { finish(&p[i], time); completed++; }
        }
        if (!ran) {
            int next_arrival = -1;
            for (int i = 0; i < n; i++) if (!p[i].is_completed &&
                (next_arrival < 0 || p[i].arrival_time < next_arrival)) next_arrival = p[i].arrival_time;
            time = next_arrival;
        }
    }
}
