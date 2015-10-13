#include "network.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"

#define BUFFER_SIZE 256

void readline(char buffer[], size_t n, FILE *stream)
{
    fgets(buffer, n, stream);
    *strchr(buffer, '\n') = '\0';
}

void calc_travel(char *from, char *to, char *start)
{
  printf("nice job mistah");
}

int main(int argc, char *argv[])
{
  
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
      if(from && start && to)
	{
	  calc_travel(from, to, start);
	}
    }
  else {
    
    network_t *n = network_parse(fopen("data.txt", "r"));
    char from[BUFFER_SIZE];
    char to[BUFFER_SIZE];
    puts("================================");
    puts(" Welcome to the travel planner! ");
    puts("================================");
    puts("");
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
	list_t *travels = network_find_travels(n, from, to);
	list_foreach(travels, (void_fun_t)travel_print);
	list_foreach(travels,free);
      }
    puts("Good bye!");
    //  network_print(n);
    return 0;
  }
  return 0;
}


