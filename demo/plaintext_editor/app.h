#ifndef APP_H
#define APP_H

#include "../../lib/data_structures/gap_buffer/gap_buffer_t.h"

typedef struct 
{
  gap_buffer_t *gap_buffer;
} app_t;

void app_render(app_t *app);

#endif
