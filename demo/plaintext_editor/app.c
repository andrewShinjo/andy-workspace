#include <stdlib.h>
#include "../../lib/data_structures/gap_buffer/gap_buffer_t.h"
#include "app.h"

#define private_function static

void app_render(app_t *app)
{
  gap_buffer_t *gap_buffer = app->gap_buffer;
  gap_buffer_slice_t slices = gap_buffer_get_slices(gap_buffer);

  // Calculate the position of each character.

  size_t text_length = gap_buffer_get_text_size(gap_buffer);
  int *x_at = malloc(text_length);
  int *y_at = malloc(text_length);
  int *width_at = malloc(text_length);
  int *height_at = malloc(text_length);

  // I needs the bounds of the editor.
}
