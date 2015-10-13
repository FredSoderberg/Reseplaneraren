#include "graph.h"
#include "list.h"
#include "stdlib.h"
#include <assert.h>
#include <stdio.h>
#include "network.h"
#include "time.h"

typedef void node_t;
typedef struct _graph_edge_t edge_t;

struct _graph_edge_t
{
    node_t *from; 
    node_t *to;
    void *label;
};

struct _graph_t
{
    list_t *nodes;
    list_t *edges;
    comparator_t comp;
};



edge_t  *new_edge()
{
    return calloc(1, sizeof(struct _graph_edge_t));
}

graph_t *graph_new(comparator_t comp)
{
    graph_t *ret = calloc(1, sizeof(struct _graph_t));

    ret->nodes = list_new();
    ret->edges = list_new();
    ret->comp = comp;

    return ret;
}

bool graph_add_node(graph_t *g, void *label)
{
    if (list_has(g->nodes, g->comp, label))
        {
            return false;
        }
    else
        {
            list_add(g->nodes, label);
            return true;
        }
}

bool graph_has_node(graph_t *g, void *label)
{
    return list_has(g->nodes, g->comp, label);
}

void graph_foreach_node(graph_t *g, void_fun_t f)
{
    list_foreach(g->nodes, f);
}

void graph_foreach_edge(graph_t *g, void_fun3_t f)
{
    iter_t *it;
    for (it = iter(g->edges); !iter_done(it); iter_next(it))
        {
            edge_t  *e = iter_get(it);
            f(e->from, e->to, e->label);
        }
    iter_free(it);
}

bool graph_add_edge(graph_t *g, void *from, void *to, void *label)
{
    assert(list_has(g->nodes, g->comp, from) && list_has(g->nodes, g->comp, to));
    edge_t  *e = new_edge();
    e->from = from;
    e->to = to;
    e->label = label;
    list_add(g->edges, e);
    return true;
}

bool graph_has_edge(graph_t *g, void *from, void *to, void **label)
{
    iter_t *it;
    for (it = iter(g->edges); !iter_done(it); iter_next(it))
        {
            edge_t  *e = iter_get(it);
	    if ((g->comp(from, e->from) && g->comp(to, e->to)) || (g->comp(from, e->to) && g->comp(to, e->from)))
	      {
		*label = e->label;
		iter_free(it);
		return true;
	      }
        }
    *label = NULL;
    iter_free(it);
    return false;
}

list_t *graph_find_neighbors(graph_t *g, void *node)
{
    list_t *ret = list_new();
    iter_t *eit;
    for (eit = iter(g->edges); !iter_done(eit); iter_next(eit))
        {
            edge_t  *e = iter_get(eit);
            if (g->comp(e->from, node))
                {
                    list_add(ret, e->to);
                }
	    else if (g->comp(e->to, node))
	      {
		list_add(ret, e->from);
	      }
        }
    iter_free(eit);
    return ret;
}

distance_label_t *get_distance_label(list_t *distanceLabels, void *lbl,
                                     comparator_t comp)
{
    assert(distanceLabels);
    assert(lbl);
    assert(comp);
    iter_t *dliter;
    for (dliter = iter(distanceLabels); !iter_done(dliter); iter_next(dliter))
        {
            distance_label_t *dl = iter_get(dliter);
            if (comp(dl->label, lbl))
                {
		  //printf("Did find distance label for %s: path is: %p\n", lbl, dl->path);

                    iter_free(dliter);
                    return dl;
                }
        }
    //       printf("did not find distance label for %s\n", lbl);
    iter_free(dliter);
    return NULL;
}

void *get_min_distance_node(list_t *distanceLabels, comparator_t comp,
                            list_t *visited)
{
    assert(distanceLabels);

    int minDist = -1;
    void *minLabel = NULL;
    iter_t *it;
    for (it = iter(distanceLabels); !iter_done(it); iter_next(it))
        {
            distance_label_t *itdl = iter_get(it);
            if (itdl->dist == -1) // inte varit där
                {
                    continue;
                }
            if (list_has(visited, comp, itdl->label)) // finns redan i visited, abort
                {
                    continue;
                }
            if (minDist == -1 || itdl->dist < minDist)// om inte ändrats en enda gång eller nya distansen 'r mindre än förra minsta distans.
                {
                    minDist = itdl->dist;
                    minLabel = itdl->label;
                }
        }
    iter_free(it);
    return minLabel;
}

void update_distance(list_t *distanceLabels, void *lbl, comparator_t comp,
                     int tentativeDist, list_t *tentativePath, list_t *tentativeEdgePath, char* new_arrival_time)//la till new_arrival_time
{
  // lägg till path_edges och arrival_time.
    assert(distanceLabels);
    assert(lbl);
    assert(comp);
    assert(tentativePath);
    distance_label_t *dl = get_distance_label(distanceLabels, lbl, comp);
    assert(dl);
    if ((dl->dist == -1) || (dl->dist > tentativeDist))
        {
            dl->dist = tentativeDist;
            if (dl->path)
                {
                    list_free(dl->path);
                }
            if (dl->path_edges)
                {
                    list_free(dl->path_edges);
                }
            dl->path = tentativePath;
	    dl->path_edges = tentativeEdgePath;
	    dl->arrival_time = new_arrival_time;  // egen rad
        }
    else
        {
            list_free(tentativePath);
	    list_free(tentativeEdgePath);
	    
        }
}

list_t *unvisited_neighbors(graph_t *g, void *current, list_t *visited)
{
    list_t *neighs = graph_find_neighbors(g, current);

    list_t *unvisited_neighs = list_new();
    iter_t *nit;
    for (nit = iter(neighs); !iter_done(nit); iter_next(nit))
        {
            void *neigh = iter_get(nit);
            if (!list_has(visited, g->comp, neigh))
                {
                    list_add(unvisited_neighs, neigh);
                }
        }
    iter_free(nit);
    list_free(neighs);
    return unvisited_neighs;
}

int graph_add_penalty(edge_t *e, char *arrival_time, char* bussDepart) // Egen funktion
{
  edge_t *temp_edge = e;
  assert(arrival_time);
  assert(bussDepart);
  int penalty = time_diff(arrival_time, bussDepart);
 
  int total_penalty = network_get_dur(temp_edge->label) + penalty;
   
  return total_penalty; 

  // bussen avgår - arrival time + duration = penalty
}

void dijkstra(graph_t *g, void *current, void *to, list_t *visited,
              list_t *distanceLabels)
{
    assert(g);
    assert(current);
    assert(to);
    assert(visited);
    assert(distanceLabels);
    while (true)
      {
	if (!list_has(visited, g->comp, current))
	  {
	    list_t *unvisited_neighs = unvisited_neighbors(g, current, visited);
	    distance_label_t *here =
	      get_distance_label(distanceLabels, current, g->comp);
	    
	    iter_t *it;
	    for (it = iter(unvisited_neighs); !iter_done(it); iter_next(it))
	      {
		void *neigh = iter_get(it);
		
		int line = list_quickest_line(g->nodes, current, neigh, here->arrival_time);
		assert(line);
		edge_t *edge = graph_get_edge(g,line,current,here->path_edges);
		
		list_t *tentativePath = list_clone(here->path);
		list_t *tentativeEdgePath = list_clone(here->path_edges);
		list_add(tentativePath, neigh);
		list_add(tentativeEdgePath, edge);

		char *bussDepart = list_next_dep_time(g->nodes,current,neigh,line,here->arrival_time);
		assert(bussDepart);
		int total_distance = graph_add_penalty(edge, here->arrival_time, bussDepart);// egen rad
		char *new_arrival_time = add_duration(bussDepart, network_get_dur(edge->label)); //egen rad
		//printf("From:%s-%s - %i - To:%s-%s \n",current,bussDepart,total_distance,neigh,new_arrival_time);				  
		update_distance(distanceLabels, neigh, g->comp, here->dist + total_distance,
				tentativePath, tentativeEdgePath, new_arrival_time); //la till new_arrival_tim. ost-bågen borde gå in här!!
	      }
	    iter_free(it);
	  }
	list_add(visited, current);
	
	if (g->comp(current, to))
	  {
	    return;
	  }
	
	current = get_min_distance_node(distanceLabels, g->comp, visited);
	assert(current);
      }
}


void graph_print_trip (graph_t *g, char *time,char *from, distance_label_t *dl)
{

  printf("\nTrip start:%s\n",time);
  char *temp_t = time;
  iter_t *it;
  char*tmp_fr;
  char*tmp_to;

  for (it = iter(dl->path_edges); !iter_done(it); iter_next(it))
    {
      void *tmp_v = iter_get(it);
      edge_t *tmp_e = tmp_v;
      int line = network_get_line(tmp_e->label);


      if(!tmp_fr) tmp_fr = from;
      if(g->comp(tmp_fr,tmp_e->from)) tmp_to = tmp_e->to;
      else{tmp_to = tmp_e->from;}
      

      
      
      char *buss_dep = list_next_dep_time(g->nodes,tmp_e->from,tmp_e->to,line,time);
      char *arr_time = add_duration(buss_dep, network_get_dur(tmp_e->label));
      int durr = network_get_dur(tmp_e->label);

      







      
      printf("@ %s: #%i %s --(%i)--> %s\n",buss_dep ,line ,tmp_e->from ,durr ,tmp_e->to );



      tmp_fr = tmp_to;
      temp_t = arr_time;
	}
    puts("_____________________________________________________________");
    printf("this trip took from:%s to:%s and %i minutes to complete.\n",time,dl->arrival_time,dl->dist);
}


distance_label_t *graph_find_path(graph_t *g,char *time, void *from, void *to)
{
    assert(graph_has_node(g, from) && graph_has_node(g, to));
    list_t *visited = list_new();
    list_t *distanceLabels = list_new();
    iter_t *it;
    for (it = iter(g->nodes); !iter_done(it); iter_next(it))
        {
            distance_label_t *dl = calloc(1, sizeof(distance_label_t));
            dl->dist = -1;
            dl->label = iter_get(it);
            dl->path = NULL;
	    dl->path_edges = NULL;
	    dl->arrival_time = time;
            list_add(distanceLabels, dl);
        }
    iter_free(it);
    get_distance_label(distanceLabels, from, g->comp)->path = list_new();
    get_distance_label(distanceLabels, from, g->comp)->path_edges = list_new();
    dijkstra(g, from, to, visited, distanceLabels);
    distance_label_t *best = get_distance_label(distanceLabels, to, g->comp);
    assert(best);
    free_distancelabels(g->comp,distanceLabels,best);
    list_free(visited);

    graph_print_trip(g,time,from,best);
    
    return best;
}

void graph_print(graph_t *g)
{
    int nsize = list_len(g->nodes);
    int esize = list_len(g->edges);

    for (size_t i = 0; i < nsize; ++i)
        {
            node_t **n;
            list_nth(g->nodes, i, (void **)&n);
        }
    for (size_t i = 0; i < esize; ++i)
        {
            edge_t  *e;
            list_nth(g->edges, i, (void **)&e);
        }
}


void graph_add_timetable(graph_t *g,char* start,int line,char* time) //Egen Funktion
{
  if(graph_has_node(g, start))
    {
      assert(g);
      assert(start);
      assert(line);
      assert(time);
      list_add_timetable(g, g->nodes, start, line, time);
    }  
}

void graph_print_timetable(graph_t *g)// Egen Funktion
{
  print_timetable(g->nodes);
  edge_t *e;
  bool has = graph_has_edge(g,"Centralstationen","Grindstugan", (void **)&e);
  if(has)puts("FINNS EN EDGE!!!!!");
}

bool graph_is_edge_visited(list_t *visited_edges, comparator_t comp, edge_t *edge)
{
  iter_t *it;
  for (it = iter(visited_edges); !iter_done(it); iter_next(it))
    {
      edge_t *temp_edge = iter_get(it);
      if (comp(temp_edge, edge))
	{
	  iter_free(it);
	  return true;
	}
    }
  iter_free(it);
  return false;
}

void *graph_get_edge(graph_t *g,int line, void *node_el, list_t *visited_edges)// Egen Funktion
{
  iter_t *it;
  for (it = iter(g->edges); !iter_done(it); iter_next(it))
    {
      edge_t  *e = iter_get(it);
      if(
	 ( (g->comp(node_el,e->from) || g->comp(node_el,e->to) )
	 &&
	   (!graph_is_edge_visited(visited_edges, g->comp, e))
	 &&
	 (network_comp_line(e->label,line))
	   ))
	{
	  void *temp_edge = e;
	  iter_free(it);
	  return temp_edge;
	}
    }
	  iter_free(it);
  return NULL;
}

char *graph_next_node_name(graph_t *g, void *edge, char *node)// Egen Funktion
{
  assert(edge);
  edge_t *temp_edge = edge;
  if(g->comp(node,temp_edge->to)) return temp_edge->from;
  if(g->comp(node,temp_edge->from)) return temp_edge->to;
  assert(false);
  return 0;
}

int graph_get_duration(void *edge)// Egen Funktion
{
  edge_t *temp_edge = edge;
  return network_get_dur(temp_edge->label);
}

bool graph_check_end_station(graph_t *g,int line, list_t *visited_edges, void *next_node) // Egen Funktion
{
  if(graph_get_edge(g,line,next_node,visited_edges) != NULL) return false;
  return true;
}

void graph_free(graph_t *g)// Egen Funktion
{
    list_free(g->nodes);
    list_free(g->edges);
}

