#include <stdio.h>
#include <string.h>
#include "storage.h"

void init_memory(MemoryBlock b[], int n) { for (int i = 0; i < n; i++) { b[i].block_id = i; b[i].size = 0; b[i].is_allocated = 0; strcpy(b[i].process_name, "FREE"); } }

int allocate_memory(MemoryBlock b[], int n, int size, const char *name, AllocationStrategy strategy) {
    int pick = -1;
    for (int i = 0; i < n; i++) if (!b[i].is_allocated && b[i].size >= size) {
        if (pick < 0 || (strategy == BEST_FIT && b[i].size < b[pick].size) || (strategy == WORST_FIT && b[i].size > b[pick].size)) pick = i;
        if (strategy == FIRST_FIT) break;
    }
    if (pick < 0) return 0;
    b[pick].is_allocated = 1; snprintf(b[pick].process_name, sizeof b[pick].process_name, "%s", name); return 1;
}

void deallocate_memory(MemoryBlock b[], int n, const char *name) { for (int i = 0; i < n; i++) if (b[i].is_allocated && !strcmp(b[i].process_name, name)) { b[i].is_allocated = 0; strcpy(b[i].process_name, "FREE"); return; } }

char *get_memory_state_string(MemoryBlock b[], int n) {
    static char out[4096]; int used = snprintf(out, sizeof out, "Block   Size (KB)    Status       Process\n"); int free_kb = 0, free_blocks = 0;
    for (int i = 0; i < n; i++) { used += snprintf(out + used, sizeof out - used, "%-7d %-12d %-12s %s\n", b[i].block_id, b[i].size, b[i].is_allocated ? "ALLOCATED" : "FREE", b[i].process_name); if (!b[i].is_allocated) { free_kb += b[i].size; free_blocks++; } }
    snprintf(out + used, sizeof out - used, "\nTotal free: %d KB | Free blocks: %d", free_kb, free_blocks); return out;
}
