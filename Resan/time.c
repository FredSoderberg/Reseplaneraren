
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "time.h"


char *new_time_list()
{
  return calloc(1, (sizeof(char)*5) + 1);
}

int time_diff(char *start_time, char *end_time)
{
  assert(start_time);
  assert(end_time);
  int hour1, min1, hour2, min2;
  sscanf(start_time, "%d:%d", &hour1, &min1);
  sscanf(end_time, "%d:%d", &hour2, &min2);

  int calc_start_time = hour1 * 60 + min1;
  int calc_end_time = hour2 * 60 + min2;

  if(calc_start_time > calc_end_time)
    {
      int diff = (1440 - calc_start_time) + calc_end_time;
      return diff;
    }
  
  if(calc_start_time <= calc_end_time)
    {
      int diff = calc_end_time - calc_start_time;
      return diff;
    }
  return 0;
}

  

int time_compare(char *time1, char *time2)
{
  assert(time1);
  assert(time2);
  int hour1, min1, hour2, min2;
  sscanf(time1, "%d:%d", &hour1, &min1);
  sscanf(time2, "%d:%d", &hour2, &min2);

  if(hour1 == hour2 && min1 == min2)
    {
      return 0;
    }
  if(hour1 > hour2 || (hour1 == hour2 && min1 > min2))
    {
      return 1;
    }
  if (hour1 < hour2 || (hour1 == hour2 && min1 < min2))
    {
      return 2;
    }

  return 0;
}

char *add_min(int hour, int min, int duration)
{
  //int duration = 60;

  char *dep = new_time_list();

  int total_time = hour + min + duration;

  int new_min = total_time % 60;

  int total_hour = total_time - new_min;

  int new_hour = total_hour / 60;

  sprintf(dep, "%02d:%02d", new_hour, new_min);
  
  return dep;
  
  /*
  int new_min = min + duration;

  char *dep = new_time_list();;

  if(new_min > 59)
    {
      int last_min = new_min - 60;
      int new_hour = hour +1;
      sprintf(dep, "%02d:%02d", new_hour, last_min);

      return dep;
    }
  
  sprintf(dep, "%02d:%02d", hour, new_min);
  */
 
}


char* add_duration(char* depart, int duration)
{
  // char *my_string = "16:02";
  // char *time1 = "16:02";
  //char *time2 = "16:01";

  int hour, min;
  
  int matches = sscanf(depart, "%d:%d", &hour, &min);

  int calc_hour = hour * 60;

  if(matches != 2) { //error handling av timestamp
    }

  //  printf("The old time is %02d:%02d\n", hour, min);
 //%02d = minst 2 siffror, varav fyller ut med nollor om det inte är två siffror.  
  char* dep = add_min(calc_hour, min, duration);

  //int cmp_time = time_compare(time1, time2);

  // printf("Time compare: %d \n", cmp_time);

  // printf("The new time is %s\n", dep);

  return dep;
}

void time_free(char *t)
{
  // TODO: FREE FUNCTION 
}
