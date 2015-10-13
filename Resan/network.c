#include "network.h"
#include "graph.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "util.h"

struct _network_t
{
    graph_t *g;
};

typedef struct _edge_t edge_t;
struct _edge_t
{
    int duration;
    line_no line;
};

#define BUFSIZE 1000

void trim_leading_space(char *dest, const char *src)
{
    assert(dest);
    assert(src);
    while (isspace(*src))
        {
            ++src;
        }
    strcpy(dest, src);
}

void printEdge(void *from, void *to, void *label)
{
    edge_t *e = label;
    printf("#%2i:%30s --(%2imin)--> %s\n", e->line, (char *)from, e->duration,
           (char *)to);
    // assert(false);
}



void timetable_parse(network_t *netw,FILE *file) // Egen funktion
{
    assert(file);

    char buffer[BUFSIZE];
    while (fgets(buffer, BUFSIZE, file))
        {
	  char bus_start[BUFSIZE];
	  int bus_line;
	  char bus_time[BUFSIZE];
	  
	  sscanf(strtok(buffer, ","), "%i", &(bus_line));
	  trim_leading_space(bus_start, strtok(NULL, ","));
	  sscanf(strtok(NULL, ","), "%s", (bus_time));
	  
	  int bus_line_dup = bus_line;
	  char *bus_start_dup = strdup(bus_start);
	  char *bus_time_dup = strdup(bus_time);

	  assert(bus_start_dup);
	  assert(bus_line_dup);
	  assert(bus_time_dup);
	  //printf("%s-%i-%s\n",bus_start_dup,bus_line_dup,bus_time_dup);

	  graph_add_timetable(netw->g, bus_start_dup, bus_line_dup, bus_time_dup);
	}
    //graph_print_timetable(netw->g);
}




network_t *network_parse(FILE *file)
{
    assert(file);
    network_t *netw = calloc(1, sizeof(struct _network_t));
    netw->g = graph_new(streq);

    char buffer[BUFSIZE];
    while (fgets(buffer, BUFSIZE, file))
        {
            char bus_from[BUFSIZE];
            char bus_to[BUFSIZE];

            edge_t *e = malloc(sizeof(struct _edge_t));

            sscanf(strtok(buffer, ","), "%i", &(e->line));
            trim_leading_space(bus_from, strtok(NULL, ","));
            trim_leading_space(bus_to, strtok(NULL, ","));
            sscanf(strtok(NULL, ","), "%i", &(e->duration));

	    //            puts("adding edge...");
	    // printEdge(bus_from, bus_to, &(e->duration));


	    
            assert(e->duration > 0 && e->duration < 100);

            char *bus_from_dup = strdup(bus_from);
            char *bus_to_dup = strdup(bus_to);

	    strlwr(bus_from_dup);
	    strlwr(bus_to_dup);
            	    
            graph_add_node(netw->g, bus_from_dup);
            graph_add_node(netw->g, bus_to_dup);
            graph_add_edge(netw->g, bus_from_dup, bus_to_dup, e);
	}

    timetable_parse(netw,fopen("start.txt","r"));
    
    return netw;
}





void network_print(network_t *n)
{
    assert(n);
    graph_foreach_edge(n->g, printEdge);
}

travel_t *travel_clone(travel_t *orig)
{
    travel_t *clone = malloc(sizeof(travel_t));
    clone->from_stop = orig->from_stop;
    clone->from_time = orig->from_time;
    clone->line = orig->line;
    clone->to_stop = orig->to_stop;
    clone->duration = orig->duration;
    return clone;
}

void travel_print(travel_t *travel)
{
    printf("@ %s: #%i %s --(%i)--> %s\n", travel->from_time, travel->line,
           travel->from_stop, travel->duration, travel->to_stop);
}

list_t *merge_rides(list_t *route)
{
    list_t *ret = list_new();
    if (list_len(route) == 1)
        {
            travel_t *travel;
            list_nth(route, 0, (void **)&travel);
            list_add(ret, travel_clone(travel));
        }
    else if (list_len(route) != 0)
        {
            travel_t *travel;
            list_nth(route, 0, (void **)&travel);

            travel_t *merged = travel_clone(travel);

            iter_t *it = iter(route);
            iter_next(it); // we already took care of the first
            for (; !iter_done(it); iter_next(it))
                {
                    travel = iter_get(it);

                    if (travel->line == merged->line)
                        {
                            merged->duration += travel->duration;
                            merged->to_stop = travel->to_stop;
                        }
                    else
                        {
                            list_add(ret, merged);
                            merged = travel_clone(travel);
                        }
                }
            iter_free(it);
            list_add(ret, merged);
        }

    return ret;
}

distance_label_t *network_find_travels(network_t *n, char *time, char *from, char *to)
{
    assert(n && from && to);
    if (streq(from, to))
        {
	  distance_label_t *dl = calloc(1, sizeof(distance_label_t));
            return dl;
        }
    distance_label_t *path = graph_find_path(n->g, time, from, to);
    return path;
}

bool network_comp_line (void *label, int line) //EGEN!!!
{
  edge_t *edge = label;
  if (edge->line == line)
    {
      return true;
    }
  return false;
}

int network_get_line (void *label)
{
  edge_t *edge = label;
  return edge->line;
}

int network_get_dur(void *label)
{
  edge_t *temp_dur = label;
  return temp_dur->duration;
}

void free_edge_label(void *from, void *to, void *label)
{
    free(label);
}

void network_free(network_t *n)
{
    graph_foreach_edge(n->g, free_edge_label);
    graph_foreach_node(n->g, free);
    graph_free(n->g);
    free(n);
}




































































