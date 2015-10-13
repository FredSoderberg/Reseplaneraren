#include "network.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include <assert.h>

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
	distance_label_t *travels = network_find_travels(n, argv[1], argv[2], argv[3]);
	assert(travels);
	puts("FUCK YEEE!!!!");
	free_dl(travels);
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
            distance_label_t *travels = network_find_travels(n,"06:00",from, to);
	    assert(travels);
	    free_dl(travels);
            //list_foreach(travels, (void_fun_t)travel_print);
	    //list_foreach(travels,free);
	  }
      }
    puts("Good bye!");
    //  network_print(n);
    return 0;
}
