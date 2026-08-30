#include <stdio.h>
#include "banker.h"

void init_banker_system(BankerSystem *s, int clients, int resources) {
    s->num_clients = clients; s->num_resources = resources;
    for (int r = 0; r < resources; r++) s->available[r] = 0;
    for (int c = 0; c < clients; c++) {
        s->clients[c].client_id = c;
        for (int r = 0; r < resources; r++) s->clients[c].allocation[r] = s->clients[c].maximum[r] = s->clients[c].need[r] = 0;
    }
}

int is_safe_state(BankerSystem *s) {
    int work[MAX_RESOURCES], done[MAX_CLIENTS] = {0}, count = 0;
    for (int r = 0; r < s->num_resources; r++) work[r] = s->available[r];
    while (count < s->num_clients) {
        int found = 0;
        for (int c = 0; c < s->num_clients; c++) if (!done[c]) {
            int possible = 1;
            for (int r = 0; r < s->num_resources; r++) if (s->clients[c].need[r] > work[r]) { possible = 0; break; }
            if (possible) { for (int r = 0; r < s->num_resources; r++) work[r] += s->clients[c].allocation[r]; done[c] = 1; count++; found = 1; }
        }
        if (!found) return 0;
    }
    return 1;
}

int request_resources(BankerSystem *s, int c, int req[]) {
    if (c < 0 || c >= s->num_clients) return 0;
    for (int r = 0; r < s->num_resources; r++) if (req[r] < 0 || req[r] > s->clients[c].need[r] || req[r] > s->available[r]) return 0;
    for (int r = 0; r < s->num_resources; r++) { s->available[r] -= req[r]; s->clients[c].allocation[r] += req[r]; s->clients[c].need[r] -= req[r]; }
    if (is_safe_state(s)) return 1;
    for (int r = 0; r < s->num_resources; r++) { s->available[r] += req[r]; s->clients[c].allocation[r] -= req[r]; s->clients[c].need[r] += req[r]; }
    return 0;
}

void release_resources(BankerSystem *s, int c, int rel[]) {
    if (c < 0 || c >= s->num_clients) return;
    for (int r = 0; r < s->num_resources; r++) { int amount = rel[r] < s->clients[c].allocation[r] ? rel[r] : s->clients[c].allocation[r]; if (amount > 0) { s->available[r] += amount; s->clients[c].allocation[r] -= amount; s->clients[c].need[r] += amount; } }
}

char *get_banker_state_string(BankerSystem *s) {
    static char out[4096]; int used = 0;
    used += snprintf(out + used, sizeof out - used, "Available: ");
    for (int r = 0; r < s->num_resources; r++) used += snprintf(out + used, sizeof out - used, "R%d=%d ", r, s->available[r]);
    used += snprintf(out + used, sizeof out - used, "\n\nClient  Allocation       Need\n");
    for (int c = 0; c < s->num_clients; c++) { used += snprintf(out + used, sizeof out - used, "C%d      ", c); for (int r = 0; r < s->num_resources; r++) used += snprintf(out + used, sizeof out - used, "%d ", s->clients[c].allocation[r]); used += snprintf(out + used, sizeof out - used, "      "); for (int r = 0; r < s->num_resources; r++) used += snprintf(out + used, sizeof out - used, "%d ", s->clients[c].need[r]); used += snprintf(out + used, sizeof out - used, "\n"); }
    return out;
}
