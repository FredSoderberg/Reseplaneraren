#include "network.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include <assert.h>
#include "graph.h"

#define BUFFER_SIZE 256

void readline(char buffer[], size_t n, FILE *stream)
{
    fgets(buffer, n, stream);
    *strchr(buffer, '\n') = '\0';
}

int main(int argc, char *argv[])
{  
  network_t *n = network_parse(fopen("data.txt", "r"));
    char from[BUFFER_SIZE];
    char to[BUFFER_SIZE];
    puts("================================");
    puts(" Welcome to the travel planner! ");
    puts("================================");
    puts("");
    if(argc > 1)
      {
	char *from;
	char *to;
	char *start;
	char *arg;
	
	for(int i = 1; i < argc; i++) //behöver inte veta filnamnet därför i = 1
	  {
	    arg = argv[i];
	    
	    if(arg[0] == '-' && arg [1] == '-')
	      {
		char *value = argv[++i];
		
		arg += 2;
		
		if(strcmp(arg, "from") == 0)
		  {
		    from = strdup(value);
		  }
		
		else if(strcmp(arg, "to") == 0)
		  {
		    to = strdup(value);
		  }
		else if(strcmp(arg, "start") == 0)
		  {
		    start = strdup(value);
		  }
		else 
		  {
		    printf("wrong input");
		    break;
		  }
	      }
	  }
	if(start && from && to)
	  {
	    if(!graph_check_exist(n, from, to))
	      {
		puts("Sorry, station does not exist. Try again.\n");
		return 0;
	      }
	    
	    distance_label_t *travels = network_find_travels(n, start, from, to);
	    assert(travels);
	    free_dl(travels);
	  }
	  
      }
    else
      {
	while(true)
	  {
            puts("What is your starting station?");
            readline(from, BUFFER_SIZE, stdin);
            if(from[0] == '\0')
	      {
		break;
	      }
            puts("Where would you like to go?");
            readline(to, BUFFER_SIZE, stdin);
	    if(!graph_check_exist(n, from, to))
	      {
		puts("Sorry, station does not exist. Try again.\n");
		continue;
	      }
	    distance_label_t *travels = network_find_travels(n,"06:00",from, to);
	    assert(travels);
	    free_dl(travels);
	  }
	
      }
    puts("Good bye!");
    //  network_print(n);
    return 0;
}


