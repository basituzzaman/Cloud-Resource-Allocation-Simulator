#ifndef GUI_H
#define GUI_H
#include <gtk/gtk.h>
#include "scheduler.h"
#include "banker.h"
#include "storage.h"
#include "job.h"

typedef struct {
    GtkWidget *window, *sched_quantum_spin;
    GtkListStore *sched_list_store, *storage_list_store;
    GtkTextBuffer *sched_output_buffer, *banker_output_buffer, *storage_output_buffer, *dashboard_output_buffer;
    Process processes[MAX_PROCESSES]; BankerSystem banker_system; MemoryBlock storage_blocks[MAX_BLOCKS];
    JobManager jobs;
    int num_processes, num_blocks, banker_initialized;
} AppData;
void gui_init(int argc, char *argv[]);
void append_text_output(GtkTextBuffer *buffer, const char *text);
void clear_text_output(GtkTextBuffer *buffer);
#endif
