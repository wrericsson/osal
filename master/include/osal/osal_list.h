
#ifndef __OSAL_LIST_H__
#define __OSAL_LIST_H__

#include "mtype.h"
typedef struct _osal_list osal_list_t;

typedef boolean (* osal_list_traverse_func)(void *data, void *user_data);

typedef boolean (* osal_list_find_func)(void *data1, void *data2);

typedef void (* osal_list_operate_func)
  (void *data1, void *data2, const void *user_data);

/*
 * We define List in this form simply because
 * in certain scenario, the list can work
 * as an unbalanced tree.
 * For example, during dynamic module linking,
 * one Stream could have tree type of linked modues,
 * in which one List Node could have multiple children(next).
 *
 * For most cases, nextNum should be ONLY one.
 *
 * It is a root node when prev == NULL
 */
struct _osal_list {
  void         *data;
  osal_list_t   *prev;
  osal_list_t   **next;   /* array of next(children) */
  uint32_t next_num; /* number of next(children) */
};

#define OSAL_LIST_PREV(osal_list) \
  ((osal_list) ? (((osal_list_t *)(osal_list))->prev) : NULL)
#define OSAL_LIST_NEXT(osal_list) \
  ((osal_list) ? (((osal_list_t **)(osal_list))->next) : NULL)

#if defined(__cplusplus)
extern "C" {
#endif

osal_list_t *osal_list_append(osal_list_t *osal_list, void *data, void *appendto,
             osal_list_find_func list_find);

// osal_list_t *osal_list_prepend(osal_list_t *osal_list, void *data);

osal_list_t *osal_list_insert(osal_list_t *osal_list, void *data, uint32_t pos);

osal_list_t *osal_list_insert_before(osal_list_t *osal_list, osal_list_t *inserted,
             const void *data);

osal_list_t *osal_list_remove(osal_list_t *osal_list, const void *data);

osal_list_t *osal_list_find_custom (osal_list_t *osal_list, void *data,
             osal_list_find_func list_find);

osal_list_t *osal_list_find_and_add_custom (osal_list_t *parent_list,
  osal_list_t *child_list, void *data, osal_list_find_func list_find);

boolean     osal_list_traverse(osal_list_t *osal_list,
              osal_list_traverse_func traverse, void *user_data);

void        osal_list_free_list(osal_list_t *osal_list);

void        osal_list_free_all(osal_list_t *osal_list,
              osal_list_traverse_func traverse);

void        osal_list_free_all_on_data(osal_list_t *osal_list,
              osal_list_traverse_func traverse, void *user_data);

void        osal_list_operate_nodes (osal_list_t *osal_list,
              osal_list_operate_func list_operate, void *user_data);

#if defined(__cplusplus)
}
#endif


#endif /* __OSAL_LIST_H__ */
