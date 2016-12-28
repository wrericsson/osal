#ifndef __OSAL_QUEUE_H__
#define __OSAL_QUEUE_H__

#include "osal_list.h"

typedef void (*osal_queue_free_func)     (void *data, void *user_data);

typedef osal_list_traverse_func osal_queue_traverse_func;

typedef struct _osal_queue {
  osal_list_t   *head;
  osal_list_t   *tail;
  uint32_t length;
} osal_queue_t;

#define OSAL_QUEUE_IS_EMPTY(q) \
  (((osal_queue_t *)(q)->head == NULL) ? TRUE : FALSE)

#define osal_queue_new malloc(sizeof(osal_queue_t));

#define OSAL_QUEUE_FIND_CUSTOM(q, data, f) \
     osal_list_find_custom((osal_queue_t*)(q)->head, (void *)d, \
     (osal_list_find_func)f)



void  osal_queue_init      (osal_queue_t *q);
void  osal_queue_free      (osal_queue_t *q);
void  osal_queue_free_all  (osal_queue_t *q, osal_queue_traverse_func traverse);
void  osal_queue_traverse  (osal_queue_t *q, osal_queue_traverse_func traverse,
        void *data);
void  osal_queue_flush     (osal_queue_t *q, osal_queue_traverse_func traverse);
void  osal_queue_push_tail (osal_queue_t *q, void *data);
void* osal_queue_pop_head  (osal_queue_t *q);
void* osal_queue_look_at_head (osal_queue_t *q);
#endif /* __OSAL_QUEUE_H__ */
