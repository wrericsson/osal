#include "mtype.h"
#include "osal_list.h"

static osal_list_t *osal_list_new(void *data)
{
  osal_list_t *new_list;

  new_list = malloc(sizeof(osal_list_t));
  if (!new_list)
    return NULL;

  new_list->data    = data;
  new_list->next    = NULL;
  new_list->next_num = 0;
  new_list->prev = NULL;

  return new_list;
}

static void osal_list_free(osal_list_t *osal_list)
{
  if (osal_list) {
    free(osal_list);
    osal_list = NULL;
  }
}

/** osal_list_append:
 *    @list - osal_list_t object;
 *    @data - new data to be appended to the list;
 *    @appendto - which node to be appendto.
 *    @func     - tree search function.
 *
 *  MctList which contains the @data;
 *  Tree model is ONLY used for a Stream to manage Modules.
 **/
osal_list_t* osal_list_append(osal_list_t *osal_list, void *data, void *appendto,
  osal_list_find_func list_find)
{
  osal_list_t *new_list = NULL;
  osal_list_t *last = NULL;

  if (!appendto && !list_find) {
    /* Normal List operation */
    new_list = osal_list_new(data);
    if (!new_list)
      return NULL;

    last = osal_list;
    if (last) {
      while (last->next)
        last = last->next[0];

      last->next = calloc(1, sizeof(osal_list_t*));
      if (!last->next)
        goto error;

      last->next[0] = new_list;
      last->next_num = 1;
      new_list->prev = last;
      return osal_list;

    } else {
      return new_list;
    }
  } else if (appendto && list_find) {

    /* This should be ONLY for Stream operates on Modules
     * when perform link or add modules */
    last = osal_list;

    if (last) {
      if (list_find(last->data, appendto) == TRUE) {
        new_list = osal_list_new(data);
        if (!new_list)
          goto error;

        if (!last->next) {
          last->next = calloc(1, sizeof(osal_list_t*));
          if (!last->next) {
            osal_list_free(new_list);
            return NULL;
          }
          last->next[0] = new_list;
          last->next_num = 1;
          new_list->prev     = last;
          return osal_list;
        } else {
          /* create tree branch */
          last->next = realloc(last->next,
            (last->next_num + 1) * sizeof(osal_list_t*));
          if (!last->next)
            goto error;

          last->next[last->next_num] = new_list;
          last->next_num++;
          new_list->prev = last;
          return osal_list;
        }
      } else if (last->next) {
        uint32_t num;
        num  = last->next_num;
        last = last->next[0];
        while (num--) {
          if (osal_list_append(last, data, appendto, list_find))
            return osal_list;
          last++;
        }
      } else {
        return NULL;
      }
    } else {
      /* Starting from an empty list */
      osal_list_t *temp = NULL;

      new_list = osal_list_new(appendto);
      if (!new_list)
        return NULL;

      new_list->next = calloc(1, sizeof(osal_list_t*));
      if (!new_list->next) {
        goto error;
      }

      temp = osal_list_new(data);
      if (!temp)
        goto error1;

      new_list->next[0] = temp;
      new_list->next_num = 1;
      temp->prev = new_list;

      return new_list;
    }
  }

error1:
  if (new_list && new_list->next)
    osal_list_free(*(new_list->next));
error:
  if (new_list)
    osal_list_free(new_list);

  return NULL;
}

#if 0
osal_list_t* osal_list_prepend(osal_list_t *osal_list, void *data)
{
  /*TO DO*/
  osal_list_t *new_list;
  int i = 0;

  new_list = osal_list_new(data);
  if (!new_list)
    return NULL;

  new_list->next = calloc(1, sizeof(osal_list_t*));
  if (!new_list->next)
    goto error;

  new_list->next[0] = osal_list;

  if (osal_list) {
    new_list->prev = osal_list->prev;

    if (osal_list->prev) {
      /*find corresponding osal_list->prev->next[i]*/
      osal_list->prev->next[i] = new_list;
    }
    osal_list->prev = new_list;
  } else
    new_list->prev = NULL;

  return new_list;
error:
  osal_list_free(new_list);
  return NULL;
}
#endif

osal_list_t* osal_list_insert(osal_list_t *osal_list __unused,
  void *data __unused, uint32_t pos __unused)
{
  /* TODO */
  return NULL;
}

osal_list_t* osal_list_insert_before(osal_list_t *osal_list __unused,
  osal_list_t *inserted __unused, const void *data __unused)
{
  /*TO DO*/
  return NULL;
}

osal_list_t* osal_list_remove(osal_list_t *osal_list, const void *data)
{
  osal_list_t *temp, *temp_list;
  uint32_t num, i, j;
  boolean moved;

  if (!osal_list)
    return osal_list;

  temp = osal_list;
  num  = osal_list->next_num;

  if (num > 0 && temp->data != data) {

    temp_list = osal_list->next[0];
    while (num--) {
      osal_list_remove(temp_list, data);
      temp_list++;
    }
  } else if (temp->data == data) {

    num = temp->next_num;

    if (temp->prev) {
      if (temp->prev->next_num == 1) {
        temp->prev->next_num = 0;
        free(temp->prev->next);
        temp->prev->next = NULL;
        if (num > 0) {
          temp->prev->next = calloc(num, sizeof(osal_list_t*));
          if (!temp->prev->next) {
            /* this is error, no action for now */
            return osal_list;
          }

          i = 0;
          while (num--) {
            temp->prev->next[i] = temp->next[i];
            temp->next[i]->prev = temp->prev;
            temp->prev->next_num++;
            i++;
          }
          free(temp->next);
          temp->next = NULL;
        } else {
          temp->prev->next = NULL;
        } /* num == 0 */
      } else if (temp->prev->next_num > 1) {
        if (num > 1) {
          temp->prev->next = realloc(temp->prev->next,
            (temp->prev->next_num + num - 1) * sizeof(osal_list_t*));
          if (!temp->prev->next) {
            /* this is error, no action for now */
            return osal_list;
          }

          i = temp->prev->next_num;
          j = 0;
          moved = FALSE;
          while (--i) { /* must use --i */
            if (moved)
              temp->prev->next[j] = temp->prev->next[j+1];

            if (temp->prev->next[j] == temp) {
              moved = TRUE;
              temp->prev->next[j] = temp->prev->next[j+1];
            }

            j++;
          }

          temp->prev->next_num--;

          i = 0;
          while (num--) {
            temp->prev->next[temp->prev->next_num] = temp->next[i];
            temp->next[i]->prev = temp->prev;
            temp->prev->next_num++;
            i++;
          }
        } else if (num == 1) {
          i = temp->prev->next_num;
          j = 0;
          while (i--) {
            if (temp->prev->next[j] == temp) {
              temp->prev->next[j] = temp->next[0];
              temp->next[0]->prev = temp->prev;
              break;
            }

            j++;
          }
        } else if (num == 0) {
          i = temp->prev->next_num;
          j = 0;
          while (--i) { /* must be --i */
            if (temp->prev->next[j] == temp) {
              temp->prev->next[j] = temp->prev->next[j+1];
            }

            j++;
          }

          temp->prev->next_num -= 1;
          temp->prev->next = realloc(temp->prev->next,
            (temp->prev->next_num) * sizeof(osal_list_t*));
          if (!temp->prev->next) {
            /* this is error, no action for now */
            return osal_list;
          }
        }
      } /* temp->prev->next_num > 1 */

      osal_list_free(temp);

    } else {
      /* temp->prev == NULL */
      if (temp->next_num == 1) {
        temp->next[0]->prev = NULL;
        osal_list = temp->next[0];
        free(temp->next);
        temp->next = NULL;
        osal_list_free(temp);
      } else if (temp->next_num == 0) {
        osal_list = NULL;
        osal_list_free(temp);
      } else {
        /*this condition is not possible*/
      }
    } /* temp->prev == NULL */
  } /* temp->data == data */

  return osal_list;
}

/** osal_list_find_custom_branch:
 *    @list: list to be traversed
 *    @data: data passed to search the particular node in the list
 *    @list_find: customized function to find node using data.
 *  Traverse whole list in recursion when the list is in form of a tree
 **/

static osal_list_t* osal_list_find_custom_branch(osal_list_t *osal_list,
  void *data, osal_list_find_func list_find)
{
  osal_list_t **temp_list;
  uint32_t num;

  if (!list_find)
    return osal_list;

  if (osal_list) {
    if (list_find(osal_list->data, data) == TRUE)
     return osal_list;

    num = osal_list->next_num;
    if (num > 0 && osal_list->next) {
      temp_list = osal_list->next;
      while (num) {
        osal_list = osal_list_find_custom_branch(temp_list[--num], data, list_find);
        if (osal_list)
          return osal_list;
      }
    }
  }

  return NULL;
}

/** osal_list_find_custom:
 *    @list: list to be traversed
 *    @data: data passed to search the particular node in the list
 *    @list_find: customized function to find node using data.
 *  Traverse whole list linearly if its not a tree.
 **/

osal_list_t* osal_list_find_custom(osal_list_t *osal_list,
  void *data, osal_list_find_func list_find)
{
  uint32_t num;

  if (!list_find)
    return osal_list;

  if (osal_list) {
    do {
      num = osal_list->next_num;
      if (num > 1) {
        osal_list = osal_list_find_custom_branch(osal_list, data, list_find);
        return osal_list;
      }
      if (list_find(osal_list->data, data) == TRUE) {
        return osal_list;
      }
      if (osal_list->next) {
        osal_list = osal_list->next[0];
      } else {
        break;
      }
    } while (osal_list != NULL);
  }
  return NULL;
}

/** osal_list_find_and_add_custom:
 *
 **/
osal_list_t* osal_list_find_and_add_custom(osal_list_t *parent_list,
  osal_list_t *child_list, void *data, osal_list_find_func list_find)
{
  osal_list_t *temp_list;
  uint32_t num;
  uint32_t idx;

  if (parent_list) {
    num = parent_list->next_num;
    if (num > 0 && parent_list->next) {
      temp_list = parent_list->next[0];
      idx = 0;
      while (num--) {
        child_list =
          osal_list_find_and_add_custom(temp_list, child_list, data, list_find);
        idx++;
        if (num)
          temp_list = parent_list->next[idx];
      }
    }
    if (list_find(parent_list->data, data) == TRUE)
      child_list = osal_list_append(child_list, parent_list->data, NULL, NULL);
  }
  return child_list;
}

/** osal_list_traverse_branch:
 *    @list: list to be traversed
 *    @traverse: customized function to find node using user_data
 *    @user_data: data passed to search the particular node in the list
 *  Traverse whole list in recursion when the list is in form of a tree
 **/

static boolean osal_list_traverse_branch(osal_list_t *osal_list,
                  osal_list_traverse_func traverse,
                  void *user_data)
{
  boolean rc = TRUE;
  osal_list_t *temp_list;
  uint32_t num;
  uint32_t idx;

  if (osal_list && traverse) {
    if (traverse(osal_list->data, user_data) == FALSE)
    return FALSE;

    num = osal_list->next_num;
    if (num > 0 && osal_list->next) {
      temp_list = osal_list->next[0];
      idx = 0;
      while (num--) {
        rc &= osal_list_traverse_branch(temp_list, traverse, user_data);
        idx++;
        if (num)
          temp_list = osal_list->next[idx];
      }
    }
  }

  return rc;
}

/** osal_list_traverse:
 *    @list: list to be traversed
 *    @traverse:customized function to find node using user_data
 *    @user_data: data passed to search the particular node in the list
 *  Traverse whole list linearly if its not a tree.
 **/

boolean osal_list_traverse(osal_list_t *osal_list,
                  osal_list_traverse_func traverse,
                  void *user_data)
{
  boolean rc = TRUE;
  uint32_t num;

  if (osal_list && traverse) {
    do {
       num = osal_list->next_num;
       if (num > 1) {
         rc = osal_list_traverse_branch(osal_list, traverse, user_data);
         break;
       }
       if (traverse(osal_list->data, user_data) == FALSE) {
          return FALSE;
       }
       if (osal_list->next) {
         osal_list = osal_list->next[0];
       } else {
         break;
       }
    } while (osal_list != NULL);
  }
  return rc;
}


/** osal_list_free_list:
 *
 **/
void osal_list_free_list(osal_list_t *osal_list)
{
  osal_list_t *temp_list;
  uint32_t num;
  uint32_t idx;

  if (osal_list) {
    num = osal_list->next_num;
    if (num > 0 && osal_list->next) {
      temp_list = osal_list->next[0];
      idx = 0;
      while (num--) {
        osal_list_free_list(temp_list);
        idx++;
        if (num)
          temp_list = osal_list->next[idx];
      }
      free(osal_list->next);
      osal_list->next = NULL;
    }
    osal_list_free(osal_list);
  }
}

/** osal_list_free_all:
 *    @list:
 *    @freefunc:
 *
 *
 **/
void osal_list_free_all(osal_list_t *osal_list, osal_list_traverse_func traverse)
{
  osal_list_traverse(osal_list, traverse, NULL);
  osal_list_free_list(osal_list);
}

/** osal_list_free_all:
 *    @list:
 *    @freefunc:
 *
 *
 **/
void osal_list_free_all_on_data(osal_list_t *osal_list,
  osal_list_traverse_func traverse, void *user_data)
{
  osal_list_traverse(osal_list, traverse, user_data);
  osal_list_free_list(osal_list);
}

/** osal_list_operate_nodes:
 *
 **/
void osal_list_operate_nodes(osal_list_t *osal_list,
  osal_list_operate_func list_operate, void *user_data)
{
  osal_list_t *temp_list;
  uint32_t num, idx;

  if (osal_list && list_operate) {
    num = osal_list->next_num;
    if (num > 0 && osal_list->next) {
      temp_list = osal_list->next[0];
      idx = 0;
      while (num--) {
        list_operate(osal_list->data, temp_list->data, user_data);
        osal_list_operate_nodes(temp_list, list_operate, user_data);
        idx++;
        if (num)
          temp_list = osal_list->next[idx];
      }
    }
  }
}
