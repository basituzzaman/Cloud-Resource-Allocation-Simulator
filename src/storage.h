#ifndef STORAGE_H
#define STORAGE_H

#define MAX_BLOCKS 100
typedef struct { int block_id, size, is_allocated; char process_name[20]; } MemoryBlock;
typedef enum { FIRST_FIT, BEST_FIT, WORST_FIT } AllocationStrategy;

void init_memory(MemoryBlock blocks[], int num_blocks);
int allocate_memory(MemoryBlock blocks[], int num_blocks, int size, const char *process_name, AllocationStrategy strategy);
void deallocate_memory(MemoryBlock blocks[], int num_blocks, const char *process_name);
char *get_memory_state_string(MemoryBlock blocks[], int num_blocks);

#endif
