#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

char *add_char(char *orig, char c)
{
  char *ret = NULL;
  char str[] = {c, '\0'};

  if (orig == NULL)
  {
    ret = malloc(2);
    memset(ret, '\0', 2);
    ret[0] = c;
  }
  else
  {
    int orig_len = strlen(orig);
    ret = realloc(orig, orig_len + 2); // one for the trailing zero, one for the next char
    memset(ret + orig_len + 1, '\0', 1);
    strncat(ret, str, 1);
  }

  return ret;
}

int check_is_keyword(char *str)
{
  for (int i = 0; i < keyword_count; i++)
  {
    if (strcmp(keywords[i], str) == 0)
    {
      return 1;
    }
  }

  return 0;
}

void fatal(char *msg, int line, int pos)
{
  printf("Error: %s, line %d pos %d\n", msg, line, pos);
  exit(1);
}