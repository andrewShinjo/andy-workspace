#ifndef APP_H
#define APP_H

#include "../../lib/data_structures/string_t.h"

typedef struct 
{
  string_t filename;
  // gap_buffer_t gap_buffer;
  size_t cursor_index;
} app_t;

#endif
