#ifndef MEMLEAK_H
#define MEMLEAK_H

#include <pthread.h>

#define PRINT_LEAK_MEMORY 1
#define MEMLEAK_EXIT 2

extern pthread_mutex_t server_memleak_mut;
extern pthread_cond_t server_memleak_con;
extern char server_memleak_event;
#ifdef __cplusplus
extern "C" {
#endif
void * server_memleak_thread (void * param __unused);
void enable_memleak_trace(int signum __unused);
#ifdef __cplusplus
}
#endif

#endif
