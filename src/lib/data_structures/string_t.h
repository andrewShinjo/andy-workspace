#ifndef STRING_T_H
#define STRING_T_H

#include <stddef.h>

typedef struct
{
  char *data;
  size_t length;
} string_t;

#endif
