#ifndef BANKER_H
#define BANKER_H

#define MAX_RESOURCES 10
#define MAX_CLIENTS 20
typedef struct { int client_id, allocation[MAX_RESOURCES], maximum[MAX_RESOURCES], need[MAX_RESOURCES]; } Client;
typedef struct { int available[MAX_RESOURCES], num_clients, num_resources; Client clients[MAX_CLIENTS]; } BankerSystem;

void init_banker_system(BankerSystem *system, int num_clients, int num_resources);
int is_safe_state(BankerSystem *system);
int request_resources(BankerSystem *system, int client_id, int request[]);
void release_resources(BankerSystem *system, int client_id, int release[]);
char *get_banker_state_string(BankerSystem *system);

#endif
