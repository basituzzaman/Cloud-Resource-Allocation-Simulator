# Cloud Resource Allocation Simulator

GTK3 desktop application for an operating-systems lab. It demonstrates CPU scheduling (FCFS, SJF, Round Robin, and priority), Banker's deadlock avoidance, and contiguous memory allocation (first, best, and worst fit).

Jobs are submitted through one form. A submission declares its CPU data, Banker maximum/requested resources, memory requirement, and allocation strategy. The application only queues the CPU process after both resource and memory allocation succeed; completing a job releases both allocations. The Dashboard tab summarizes grants, denials, scheduling averages, memory utilization, and fragmentation.

### Build

Install GTK3 development headers and `pkg-config`, then run:

```sh
make
make run
```

On Debian/Ubuntu, dependencies are available as `libgtk-3-dev` and `pkg-config`.
