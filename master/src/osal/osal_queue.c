#include "mtype.h"
#include "osal_queue.h"

void osal_queue_init(osal_queue_t *q)
{
  if (!q)
    return;

  q->head = q->tail = NULL;
  q->length = 0;
}

void osal_queue_free(osal_queue_t *q)
{
  if (!q)
    return;

  osal_list_free_list(q->head);
  free(q);
  q = NULL;
}

void osal_queue_free_all(osal_queue_t *q, osal_queue_traverse_func traverse)
{
  if (!q)
    return;

  osal_list_free_all(q->head, traverse);
  free(q);
  q = NULL;
}

void osal_queue_traverse(osal_queue_t *q, osal_queue_traverse_func traverse,
  void *data)
{
  if (!q || !traverse)
    return;

  osal_list_traverse(q->head, traverse, data);
}

/**
 *
 **/
void osal_queue_push_tail(osal_queue_t *q, void *data)
{
  if (!q)
    return;

  q->tail = osal_list_append(q->tail, data, NULL, NULL);

  if (!q->tail)
    return;

  if (q->tail->next)
    q->tail = q->tail->next[0];
  else
    q->head = q->tail;

  q->length++;
}

void *osal_queue_pop_head(osal_queue_t *q)
{
  if (!q)
    return NULL;

  if (q->head) {
    osal_list_t *node = q->head;
    void    *data = node->data;

    if (node->next) {
      q->head = node->next[0];
      free(node->next);
      node->next = NULL;
    } else {
      q->head = NULL;
    }
    if (q->head) {
      q->head->prev = NULL;
    } else {
      q->tail = NULL;
    }

    free(node);
    node = NULL;
    q->length--;

    return data;
  }

  return NULL;
}

/*  osal_queue_look_at_head
 *
 *  Description:
 *  Returns the data from the head of the queue.
 *  Does not add/remove anything in queue.
 **/
void *osal_queue_look_at_head(osal_queue_t *q)
{
  if (!q)
    return NULL;
  if (q->head)
    return q->head->data;
  else
    return NULL;
}

void osal_queue_flush(osal_queue_t *q, osal_queue_traverse_func traverse)
{
  if (!q)
    return;

  osal_list_free_all(q->head, traverse);
  osal_queue_init(q);
  return;
}
