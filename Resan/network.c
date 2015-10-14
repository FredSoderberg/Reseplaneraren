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

bool trim_leading_space(char *dest, char *src)
{
    assert(dest);
    assert(src);
    while (isspace(*src))
        {
            ++src;
        }
    if(strlen(src) == 0)return false;
    strcpy(dest, src);
    return true;
}

void printEdge(void *from, void *to, void *label)
{
    edge_t *e = label;
    printf("#%2i:%30s --(%2imin)--> %s\n", e->line, (char *)from, e->duration,
           (char *)to);
    // assert(false);
}

void test(){}

bool check_time(char *time)
{
  int hour,min;
  if(sscanf(time, "%d:%d", &hour, &min))return true;
  return false;
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
	  
	  if( !(sscanf(strtok(buffer, ","), "%i", &(bus_line)))) continue;

	  char *tmp_bs = strtok(NULL, ",");
	  if( !trim_leading_space(bus_start,tmp_bs)) continue;
	  

	  char *tmp_time = strtok(NULL, ",");
	  if( !trim_leading_space(bus_time,tmp_time)) continue;
	  if( !check_time(tmp_time))continue;
	  
	  int bus_line_dup = bus_line;
	  char *bus_start_dup = strdup(bus_start);
	  char *bus_time_dup = strdup(bus_time);

	  
	  
	  graph_add_timetable(netw->g, bus_start_dup, bus_line_dup, bus_time_dup);
	}
    fclose(file);
    //STÄNG FILEN
    // graph_print_timetable(netw->g);
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
            	    
            graph_add_node(netw->g, bus_from_dup);
            graph_add_node(netw->g, bus_to_dup);
            graph_add_edge(netw->g, bus_from_dup, bus_to_dup, e);
	}

    fclose(file);
    //STÄNG FILEN
    timetable_parse(netw,fopen("start.txt","r"));
    
    return netw;
}





void network_print(network_t *n)
{
    assert(n);
    graph_foreach_edge(n->g, printEdge);
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




































































