#ifndef FDLEAK_H
#define FDLEAK_H

#include <pthread.h>

#define DUMP_FDLEAK_TRACE 1
#define FDLEAK_EXIT 2

extern pthread_mutex_t osal_debug_fdleak_mut;
extern pthread_cond_t osal_debug_fdleak_con;
extern char osal_debug_fdleak_event;
#ifdef __cplusplus
extern "C" {
#endif
void * osal_debug_fdleak_thread (void * param __unused);
void osal_degug_enable_fdleak_trace();
void osal_degug_dump_fdleak_trace();
#ifdef __cplusplus
}
#endif

#endif
