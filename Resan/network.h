/**
  \file network.h
  \brief Functions for route planning.

  This module is a thin wrapper around a generic graph implementation.
  It adds the ability to parse transportation networks from a \ref
  FILE*, and to find travels.

*/

#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include "list.h"
#include "graph.h"

/**
   An opaque type for a transportation network.
*/
typedef struct _network_t network_t;

typedef char *time;

typedef int line_no;

/**
 Parse a network from a file. The function expects an open file and
 will not close the file for you.
*/

network_t *network_parse(FILE *file);

/**
   Prints a version of the network to stdout. For debugging purposes
   only.
*/
void network_print(network_t *);

/**
   Finds a travel from one stop to another.
   
   \returns
   - NULL or an empty list if there is no such travel.
   - A list containing Travels, otherwise
*/

distance_label_t *network_find_travels(network_t *n, char *time, char *from, char *to);

/**
   Checks if the sations supplied by the user exists.

   \param from Start station for user.

   \param to End station for user.
   
   \return true if the supplied station exists in the graph.						\

 */


bool graph_check_exist(network_t *n, char* from, char* to);



/**
   Checks if the supplied busline has the supplied route.

   \param label The route.

   \param line The busline.

   \returns true if the supplied busline has the supplied route.


 */

bool network_comp_line (void *label, int line);

/**
   Find how long it takes for a bus to travel from a certain station to another.

   \param label The route of the bus

   \returns the duration of the bus.

 */

int network_get_dur(void *label);

/**
   Find the busline for the supplied route. 

   \param label The route of the bus.

   \returns the busline.
 */

int network_get_line (void *label);

/**
Oanvänd funktion
 */
//int print_edge_duration(void *e);

/**
  Frees a network and all the data it allocated.
*/
void network_free(network_t *);

#endif // NETWORK_H
