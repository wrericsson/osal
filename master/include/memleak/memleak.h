#ifndef MEMLEAK_H
#define MEMLEAK_H

#include <dlfcn.h>
#include <stdio.h>
#include <ctype.h>
#include <unwind.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h> 

#include <pthread.h>

#define PRINT_LEAK_MEMORY 1
#define MEMLEAK_EXIT 2
#define MEMLEAK_FLAG
#define ENABLE_SIG 1
#define SIG_PRINT 28
#define SIG_ENABLE 29
#define SIG_DISABLE 30

#define MAX_BACKTRACE_DEPTH 15
#define MAX_FUNC_STRING_LENGHT 100
#define MAGIC_ALLOC 0x7abc0fb5
#define MAGIC_FREE 0x087cbc8a
struct hdr_t {
  int alloc_traced;
  unsigned int size;
  struct hdr_t *prev;
  struct hdr_t *next;
  uintptr_t bt[MAX_BACKTRACE_DEPTH];
  int bt_depth;
  int allocated;
} __attribute__((packed));
typedef struct hdr_t hdr_t;

struct map_info_holder {
  char *name;
  struct map_info_holder* next;
  uintptr_t start;
  uintptr_t end;
};

struct stack_crawl_state_t {
  uintptr_t *addr;
  int skip;
  size_t stack_count;
  size_t max_depth;
};


extern pthread_mutex_t server_memleak_mut;
extern pthread_cond_t server_memleak_con;
extern char server_memleak_event;
#ifdef __cplusplus
extern "C" {
#endif
void * server_memleak_thread (void * param __unused);
void enable_memleak_trace(int signum __unused);
int function_stacktrace(uintptr_t *addrs,size_t max_entries);

void lib_map_destroy(struct map_info_holder *map_hold);
struct map_info_holder *lib_map_create(pid_t pid); 
void print_backtrace(struct map_info_holder *p_map_info, uintptr_t* frames, int frame_count);

#ifdef __cplusplus
}
#endif

#endif
