#include "stdbool.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

bool streq(const void *a, const void *b)
{
    return (0 == strcmp((const char *)a, (const char *)b));
}

void printstr(void *str)
{
    puts((char *)str);
}

void strlwr(char *str) // EGEN FUNKTION
{
  int i = 0;

  while(str[i])
    {
      str[i] = tolower(str[i]);
      i++;
    }
}
