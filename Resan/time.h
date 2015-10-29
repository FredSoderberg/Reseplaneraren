/** 

  \file time.h
  \brief Functions for time calculations

  This module adds the abilty to takes strings in the form of HOUR:MIN (24 hr time format) and apply time related operations to them. */

/**
   
   Takes two times and calculates the time difference. 
   
   
   \returns  the time difference
   
*/
int time_diff(char *time1, char *time2);

/**

   Takes two times and compares them according time difference


   \returns and int that depends on wherever the two times are the same or the first time is bigger than the second and vice versa. 
   
*/

int time_compare(char *time1, char *time2);

/**
   
   Combines hour min and dur to a new time.

   \param hour base hour to use.

   \param min base minute to use.

   \param dur the duration in minutes to be added to hour and min
   
   \returns A newly heap-allocated string on the format HH:MM.

*/
char *add_min(int hour, int min, int dur);

/**
   Combines the value of dur and the string time.
   
   \param time on format HH:MM.
   
   \param dur minutes to be added.
   
   \returns A newly heap-allocated string.  
*/
char *add_duration(char *time, int dur);

  
