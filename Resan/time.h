/** 

  \file time.h
  \brief Functions for time calculations

  This module adds the abilty to takes strings in the form of HOUR:MIN (24 hr time format) and apply time related operations to them. */

/**
   
   Takes two strings, converts them to integers and calculates the time difference of the two integers.
   
   \param time1. string that represent a value on format HH:MM.
   
   \param time1. string that represent a value on format HH:MM.
   
   \returns returns the total time difference in integer form.
   
*/
int time_diff(char *time1, char *time2);

/**

   Takes two strings, converts them to integers. We then compare the two integers.

   \param time1. string that represent a value on format HH:MM.

   \param time1. string that represent a value on format HH:MM.

   \returns the int 0 if the both times are equal. Returns the int 1 if time1 is larger then time 2. Returns the int 2 if time2 is larger then time1.
   
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

  
