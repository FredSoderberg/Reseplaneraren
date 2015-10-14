#include "list.h"
#include <stdlib.h>
#include "assert.h"
#include <stdio.h>
#include "graph.h"
#include "time.h"

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
  char *from;
  char *departs;
  char *next_stop;
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
    iter_free(it);
    return ret;
}


char* concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}



void list_add_time(time_list_t *l, int line, char* time, char* start, char* next_node) //Egen funktion
{
  assert(l);
  assert(line);
  assert(time);  
  timetable_t *temp_table;
 
  if(l->first && l->last) //list exists
    {
      for(temp_table = l->first; temp_table != NULL; temp_table = temp_table->next)
	  {
	  if( (temp_table->line == line)
	      &&
	      (strncmp(temp_table->from,start,100) == 0)
	      )
	    {
	      char* tmp1 = concat(" ",time);
	      char* tmp2 = concat(temp_table->departs,tmp1);
	      free(temp_table->departs);
	      free(tmp1);
	      temp_table->departs = tmp2;
	      
	      return;
	    }
	  else if (temp_table->next == NULL)
	    {
	      timetable_t *new_last = timetable_new();
	      new_last->line = line;
	      new_last->departs = time;
	      new_last->from = start;
	      new_last->next_stop = next_node;
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
      new_last->departs = time;
      new_last->from = start;
      new_last->next_stop = next_node;
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
      free(list_find_node(nodes,temp_node->element)->timetable->last->destination);
      list_find_node(nodes,temp_node->element)->timetable->last->destination = strdup(end_station_el);
      temp_node = temp_node->next;
    }
}

void list_add_timetable(void *g, list_t *nodes, char* start, int line, char* time) //Egen funktion
{
  assert(g);
  assert(nodes);
  assert(start);
  assert(line);
  assert(time);
  list_node_t *node =list_find_node(nodes,start);
  list_t *visited_nodes = list_new();
  list_t *visited_edges = list_new();
  bool end_station = false;
  char *next_node;
  time = add_duration(time,0);
  list_add_time(node->timetable,line,time,start,"Temp");
  while(!end_station)
    {
      void *edge = graph_get_edge(g, line, node->element, visited_edges);
      assert(edge);
      next_node = graph_next_node_name(g,edge,node->element);
      node->timetable->last->next_stop = next_node;
      list_add(visited_nodes,node->element);
      list_add(visited_edges,edge);
      node = list_find_node(nodes,next_node);
      int duration = graph_get_duration(edge);
      char *new_time = add_duration(time,duration);
      end_station = graph_check_end_station(g, line, visited_edges, next_node);
      if(end_station)
	{
	  list_add_destination(nodes, next_node,visited_nodes);
	}
      else
	{
	  list_add_time(node->timetable,line, new_time, start, next_node);
	  time = new_time;
	}
    }
  list_free(visited_nodes);
  list_free(visited_edges);    
}
 
void print_timetable(list_t *l)
{
  iter_t *it;
  for (it = iter(l); !iter_done(it); iter_next(it))
    {
      //      printf("\n%s",it->cur->element);

      if(strncmp(iter_get(it),"Centralstationen",100) == 0)
	{
	  timetable_t *cur = it->cur->timetable->first;
	  while(cur)
	    {
	      
	      if((cur->line == 111) && (strncmp(cur->next_stop,"Grindstugan",100) == 0) )
		{
		  printf("\nLinje:%i Mot:%s Next Node:%s\nTider:%s\n",cur->line,cur->destination,cur->next_stop,cur->departs);
		}
	      cur = cur->next;
	    }
	}
    }
  iter_free(it);
}

char *list_read_timetable(timetable_t *timetable, char *start_time)
{
  assert(start_time);
  char *temp_timetable = strdup(timetable->departs);
  char *comp_time = strtok(temp_timetable," ");
  while(true)
    {
      switch (time_compare(start_time,comp_time))
	{
	case 0:
	  return start_time;
	case 1:
	  comp_time = strtok(NULL," ");
	  if(comp_time == NULL)
	    {
	      comp_time = strtok(strdup(timetable->departs)," ");
	      assert(comp_time);
	      return comp_time;
	    }
	  break;
	case 2:
	  return comp_time;
	}
    }
  assert(false);
  return 0;
 
}

char *list_next_dep_time(list_t *nodes, char *from_node_el,char *to_node_el, int line, char *start_time)
{
  assert(nodes);
  assert(from_node_el);
  assert(to_node_el);
  assert(line);
  assert(start_time);
  list_node_t *from_node = list_find_node(nodes,from_node_el);
  timetable_t *temp_timetable = from_node->timetable->first;
  assert(from_node);
  assert(temp_timetable);
 
  while(temp_timetable)
    {
      if( (temp_timetable->line == line)
	  &&
	  (strncmp(temp_timetable->next_stop,to_node_el,100) == 0))
	{
	  return list_read_timetable(temp_timetable,start_time);
	}
      temp_timetable = temp_timetable->next;
    }
  assert(false);
  return 0;
}

int list_quickest_line(list_t *nodes,char *from_node_el,char *to_node_el, char *start_time)
{
  assert(nodes);
  assert(from_node_el);
  assert(to_node_el);
  assert(start_time);
  list_node_t *from_node = list_find_node(nodes,from_node_el);
  assert(from_node);
  timetable_t *temp_tb;
  int ql = 0;
  char *c_time;
  char *n_time;
  for(temp_tb = from_node->timetable->first; temp_tb != NULL; temp_tb = temp_tb->next)
    {
      if(strncmp(temp_tb->next_stop,to_node_el,1000) == 0)
	{
	  assert(temp_tb);
	  if(!ql)c_time =list_read_timetable(temp_tb,start_time); 
	  if(!ql)ql = temp_tb->line;
	  if(ql)n_time = list_read_timetable(temp_tb,start_time);
	  if(time_compare(n_time,c_time) == 2)
	    {
	      c_time = n_time;
	      ql = temp_tb->line;
	    }
	  assert(c_time);
	  assert(ql);
	}
    }
  assert(ql);
  return ql;
}


void free_dl(distance_label_t *to_free)
{

  if (to_free->path != NULL) list_free(to_free->path);
  if (to_free->path_edges != NULL) list_free(to_free->path_edges);
  free(to_free);
}

void list_timetable_free(time_list_t *l)
{
    assert(l);
    timetable_t *cur = l->first;
    while (cur)
        {
            timetable_t *to_delete = cur;
            cur = cur->next;
            free(to_delete);
        }
    free(l);
}

void free_distancelabels(comparator_t comp,list_t *dl_list, distance_label_t *dl)
{
  list_node_t *temp_dl = dl_list->first;
  while(temp_dl)
    {
      if(temp_dl->timetable !=NULL)list_timetable_free(temp_dl->timetable);
      if(temp_dl->element == dl)
	{
	  temp_dl = temp_dl->next;
	  continue;
	}
      distance_label_t *to_free = temp_dl->element;
      temp_dl = temp_dl->next;
      free_dl(to_free);
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
	    list_timetable_free(to_delete->timetable);
            free(to_delete);
        }
    free(l);
}
