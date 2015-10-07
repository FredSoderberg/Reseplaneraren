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
/*
void time_list_add(time_list_t *l) // Egen funktion
{
  assert(l);
  assert((l->first && l->last) || (!l->first && !l->last));
  timetable_t *new_last = timetable_new();
  new_last->destination = "Empty";
  
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
}*/


void list_add(list_t *l, void *elt)
{
    assert(l);
    assert((l->first && l->last) || (!l->first && !l->last));

    list_node_t *new_last = list_node_new();
    new_last->element = elt;
    new_last->timetable = time_list_new(); // Egen Rad!!
    //time_list_add(new_last->timetable);// Egen Rad

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
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}



void list_add_time(time_list_t *l, int line, char* time) //Egen funktion
{
  assert(l);
  assert(line);
  assert(time);  
  timetable_t *temp_table;
 
  if(l->first && l->last) //listan exist
    {
      for(temp_table = l->first; temp_table != NULL; temp_table = temp_table->next)
	{
	  if(temp_table->line == line)// && (strncmp(temp_table->destination,"Empty",100) == 0) )
	    {
	      char* temp= concat(" ",time);
	      temp_table->departs = concat(temp_table->departs,temp);
	      return;
	    }
	  else if (temp_table->next == NULL)
	    {
	      timetable_t *new_last = timetable_new();
	      new_last->line = line;
	      new_last->departs = time;
	      new_last->destination = "Empty";
	      l->last->next = new_last;
	      l->last = new_last;
	      return;
	    }	  
	}
    }
  else //list is empty
    {
      timetable_t *new_last = timetable_new();
      new_last->line = line;
      new_last->destination = "Empty";
      new_last->departs = time;
      l->first = l->last = new_last;
      return;
    }
  assert(false);
}
  

list_node_t *list_find_node(list_t *nodes, char* match)
{
  assert(nodes);
  list_node_t *cur = nodes->first;
  while(cur)
    {
      if (strncmp(cur->element,match, 100) == 0)  return cur;
      cur = cur->next;
    }
  assert(false);
  return NULL;
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

void list_add_destination(list_t *nodes, char *end_station_el, list_t *visited_nodes)
{
  list_node_t *temp_node = visited_nodes->first;
  while (temp_node)
    {
      list_find_node(nodes,temp_node->element)->timetable->last->destination = strdup(end_station_el);
      temp_node = temp_node->next;
    }
}

void list_add_timetable(void *g, list_t *nodes, char* start, int line, char* time) //Egen funktion
{
  list_node_t *node =list_find_node(nodes,start);
  list_t *visited_nodes = list_new();
  list_t *visited_edges = list_new();
  bool end_station = false;
  char *next_node;
  list_add_time(node->timetable,line,time);

  while(!end_station)
    {
      void *edge = graph_get_edge(g, line, node->element, visited_edges);
      assert(edge);
      next_node = graph_next_node_name(g,edge,node->element);
      list_add(visited_nodes,node->element);
      list_add(visited_edges,edge);
      node = list_find_node(nodes,next_node);

    
      //int duration = graph_get_duration(edge);
      list_add_time(list_find_node(nodes,next_node)->timetable,line,"05:00");
	  
      end_station = graph_check_end_station(g, line, visited_edges, next_node);
      if(end_station) list_add_destination(nodes, next_node,visited_nodes);
    }
}
 

void print_timetable(list_t *l)
{
  iter_t *it;
  for (it = iter(l); !iter_done(it); iter_next(it))
    {
      printf("%s",it->cur->element);
      timetable_t *cur = it->cur->timetable->first;
      while(cur)
	{
	  printf("\nLinje:%i Mot:%s\nTider:%s\n",cur->line,cur->destination,cur->departs);
	  cur = cur->next;
	}
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
