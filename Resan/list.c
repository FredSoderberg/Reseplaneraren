#include "list.h"
#include "stdlib.h"
#include "assert.h"
#include <stdio.h>

typedef struct _list_node_t list_node_t;
typedef struct _timetable_t timetable_t;
typedef struct _time_list_t time_list_t;


struct _list_node_t
{
  void *element;
  struct _time_list_t *timetable;
  struct _list_node_t *next;

  
};

struct _list_t
{
    list_node_t *first;
    list_node_t *last;
};

struct _time_list_t
{
  timetable_t *first;
  timetable_t *last;
};

struct _timetable_t
{
  int *line;
  void *destination;
  void *departs;
  struct _timetable_t *next;
};

// PRIVATE ===========================================================

list_node_t *list_node_new()
{
    return calloc(1, sizeof(struct _list_node_t));
}

// PUBLIC ============================================================

list_t *list_new()
{
    return calloc(1, sizeof(struct _list_t));
}

int list_len(list_t *l)
{
    int ret = 0;
    assert(l);
    list_node_t *cur = l->first;
    while (cur)
        {
            cur = cur->next;
            ret++;
        }
    return ret;
}

void list_add(list_t *l, void *elt)
{
    assert(l);
    assert((l->first && l->last) || (!l->first && !l->last));

    list_node_t *new_last = list_node_new();
    new_last->element = elt;

    if (l->first && l->last)
        {
            l->last->next = new_last;
            l->last = new_last;
        }
    else
        {
            // list is empty
            l->first = l->last = new_last;
        }
}

bool list_nth(list_t *l, int n, void **elt)
{
    assert(l);
    if (n < 0)
        {
            *elt = NULL;
            return false;
        }
    else if (n >= list_len(l))
        {
            *elt = NULL;
            return false;
        }
    else
        {
            list_node_t *cur = l->first;
            while (n != 0)
                {
                    cur = cur->next;
                    n--;
                }
            *elt = cur->element;
            return true;
        }
}

bool list_has(list_t *l, comparator_t comp, void *elt)
{
    assert(l);
    list_node_t *cur = l->first;
    while (cur)
        {
            if (comp(cur->element, elt))
                {
                    return true;
                }
            cur = cur->next;
        }
    return false;
}

void list_foreach(list_t *l, void_fun_t f)
{
    iter_t *it;
    for (it = iter(l); !iter_done(it); iter_next(it))
        {
            f(iter_get(it));
        }
    iter_free(it);
}

struct _iter_t
{
    list_node_t *cur;
};

iter_t *iter(list_t *l)
{
    assert(l);
    iter_t *it = calloc(1, sizeof(struct _iter_t));
    it->cur = l->first;
    return it;
}

void iter_next(iter_t *it)
{
    assert(it);
    if (it->cur)
        {
            it->cur = it->cur->next;
        }
}

bool iter_done(iter_t *it)
{
    assert(it);
    return !it->cur;
}

void *iter_get(iter_t *it)
{
    assert(it && it->cur);
    return it->cur->element;
}

void iter_free(iter_t *it)
{
    free(it);
}

list_t *list_clone(list_t *l)
{
    assert(l);
    list_t *ret = list_new();
    iter_t *it;
    for (it = iter(l); !iter_done(it); iter_next(it))
        {
            list_add(ret, iter_get(it));
        }
    return ret;
}


bool list_has_timetable(time_list_t *l, int line)
{
  if(l->first != NULL)
    {
      timetable_t *temp_table;
      for(temp_table = l->first; temp_table->next == NULL; temp_table = temp_table->next)
	{
	  int *temp_line = temp_table->line;
	  if(*temp_line == line)
	    {
	      return true;
	    }
	}
    }
  return false;
}

//TODO Funktion1: Ifall den existerar
//funktion 2: ifall den inte existerar

void list_add_timetable(list_t *nodes, char* start, int line, char* time) //Egen funktion
{
  iter_t *it;
  for (it = iter(nodes); !iter_done(it); iter_next(it))
    {
      if (strncmp(iter_get(it),start,30) == 0)
	{
	  
        
	  
	}
    }
  iter_free(it);
}




void list_free(list_t *l)
{
    assert(l);
    list_node_t *cur = l->first;
    while (cur)
        {
            list_node_t *to_delete = cur;
            cur = cur->next;
            free(to_delete);
        }

    free(l);
}
