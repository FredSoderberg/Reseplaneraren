#include "list.h"
#include <stdlib.h>
#include "assert.h"
#include <stdio.h>
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
      else if (temp_table->next == NULL)
	{
	  timetable_t *new_last = timetable_new();
	  new_last->line = line;
	  new_last->departs = time;
	  l->last->next = new_last;
	  l->last = new_last;
	}
    }
}


list_node_t *list_find_node(list_t *nodes, char* match)
{
  assert(nodes);
  iter_t *it;
  for (it = iter(nodes); !iter_done(it); iter_next(it))
    {
      if (strncmp(iter_get(it),match, 50) == 0)
	{
	  return it->cur;
	}
    }
  return NULL;
}
void test()
{
}

void print_test(list_t *visited)
{
  list_node_t *cur = visited->first;
  while(cur)
    {
      printf("Visited:%s\n",cur->element);
      cur = cur->next;
    }
}

void list_add_timetable(void *g, list_t *nodes, char* start, int line, char* time) //Egen funktion
{
  list_node_t *node =list_find_node(nodes,start);
  list_t *visited_nodes = list_new();
  list_t *visited_edges = list_new();
  bool end_station = false;
  char *next_node;
  
  list_add(visited_nodes,node->element);
  list_add_time(node->timetable,line,time);
  // puts("hej");

  while(!end_station)
    {
      void *edge = graph_get_edge(g, line, node->element, visited_edges);
      if(edge != NULL)
	{
	  //int duration = graph_get_duration(edge);
	  
	  next_node = graph_next_node_name(g,edge,node->element);
	  //print_edge(edge);
	  //printf("Time:%i - NOW:%s - NEXT:%s\n",duration, node->element,next_node);

	  
	  
	  list_add_time(list_find_node(nodes,next_node)->timetable,line,"05:00");
	  
	  list_add(visited_nodes,node->element);

	  // print_test(visited_nodes);

	  
	  list_add(visited_edges,edge);
	  node = list_find_node(nodes,next_node);
	}
      end_station = graph_check_end_station(g, line, visited_edges, visited_nodes, next_node);
      //lägga till tiderna i noden framför!
      //  list_add(visited,next_node);
    }
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
