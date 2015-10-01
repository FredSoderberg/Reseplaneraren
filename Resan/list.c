#include "list.h"
#include "stdlib.h"
#include "assert.h"
#include <stdio.h>
#include <time.h>
#include "graph.h"

typedef struct _list_node_t list_node_t;
typedef struct _timetable_t timetable_t;
typedef struct _time_list_t time_list_t;


struct _list_node_t
{
  void *element;
  time_list_t *timetable;
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
  int line;
  char *destination;
  char *departs;
  struct _timetable_t *next;
};



// PRIVATE ===========================================================

list_node_t *list_node_new()
{
    return calloc(1, sizeof(struct _list_node_t));
}

timetable_t *timetable_new() // Egen FUnktion
{
    return calloc(1, sizeof(struct _timetable_t));
}

time_list_t *time_list_new() // Egen FUnktion
{
    return calloc(1, sizeof(struct _time_list_t));
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

void time_list_add(time_list_t *l) // Egen funktion
{
  assert(l);
  assert((l->first && l->last) || (!l->first && !l->last));
  timetable_t *new_last = timetable_new();

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


void list_add(list_t *l, void *elt)
{
    assert(l);
    assert((l->first && l->last) || (!l->first && !l->last));

    list_node_t *new_last = list_node_new();
    new_last->element = elt;
    new_last->timetable = time_list_new(); // Egen Rad!!
    time_list_add(new_last->timetable);// Egen Rad

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


char* concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}



void list_add_time(time_list_t *l, int line, char* time) //Egen funktion
{
  timetable_t *temp_table;
  for(temp_table = l->first; temp_table != NULL; temp_table = temp_table->next)
    {
      if(temp_table->line == line)
	{
	  char* temp= concat(" ",time);
	  char* s = concat(temp_table->departs,temp);
	  temp_table->departs = s;
	  return;
	}
      else
	{
	  temp_table->line = line;
	  temp_table->departs = time;
	  return;
	}
    }
}

void list_time_adder (void *g, list_node_t *node, int line, char* time)
{
  list_t *visited = list_new();
  char *node_next = NULL;
  


  list_add(visited,node);
  graph_find_duration(g, time, line, node->element, node_next, visited); //Funkar returnerar tiden å adderar på visited samt ändrar noden till "nästa"
  printf("%s ",node_next);

  //get duration
  //get next node
  


  
  //list_has_timetable(node->timetable,line,time);
}

void list_add_timetable(void *g, list_t *nodes, char* start, int line, char* time) //Egen funktion
{
  assert(nodes);
  iter_t *it;
  for (it = iter(nodes); !iter_done(it); iter_next(it))
    {
      if (strncmp(iter_get(it),start,30) == 0)
	{
	  assert(it->cur->timetable->first);
	  list_add_time(it->cur->timetable,line,time);
	  list_time_adder(g, it->cur,line,time);
	}
    }
  iter_free(it);
}

void print_timetable(list_t *l)
{
  iter_t *it;
  for (it = iter(l); !iter_done(it); iter_next(it))
    {
      puts("");
      puts(it->cur->element);
      printf("%i - %s\n",it->cur->timetable->first->line,it->cur->timetable->first->departs);
    }
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
