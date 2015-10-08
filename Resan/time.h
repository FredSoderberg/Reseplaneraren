/** 

  \file time.h
  \brief Functions for time calculations

  For such time calculations as adding minutes to a given time and comparing different times against each other. */


char *new_time_list();
/**

  Compares two given times.

  \returns an int depending on the outcome

*/

int time_compare(char *time1, char *time2);

/**

  Add minutes to a given time.

  \returns given time with given minutes added

*/
char *add_min(int hour, int min, int duration);

char *add_duration(char * depart, int duration);

  
