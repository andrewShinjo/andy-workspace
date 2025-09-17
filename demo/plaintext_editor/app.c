#include <stdio.h>
#include <stdlib.h>
#include "../../lib/data_structures/gap_buffer/gap_buffer_t.h"
#include "../../platform/platform.h"
#include "app.h"

#define private_function static

void app_render(app_t *app)
{
  gap_buffer_t *gap_buffer = app->gap_buffer;
  rectangle_t rectangle = app->rectangle;
  gap_buffer_slice_t slices = gap_buffer_get_slices(gap_buffer);

  size_t text_length = gap_buffer_get_text_size(gap_buffer);
  int *x_at = malloc(text_length * sizeof(int));
  int *y_at = malloc(text_length * sizeof(int));
  int *width_at = malloc(text_length * sizeof(int));
  int *height_at = malloc(text_length * sizeof(int));

  int current_x = rectangle.x;
  int current_y = rectangle.y;

  // Calculate the position of each character on the left slice.
  for(int i = 0; i < slices.left_length; i++)
  {
    char c = *(slices.left_start + i);
    int char_height = platform_get_text_height(&c, 1);
    int char_width = platform_get_text_width(c, 1);

    if(current_x + char_width > rectangle.x + rectangle.width)
    {
      current_x = rectangle.x;
      current_y += char_height;
    }

    x_at[i] = current_x;
    y_at[i] = current_y;
    width_at[i] = char_width;
    height_at[i] = char_height;

    current_x += char_width;
  }


  // Draw each character on the left slice.
  for(int i = 0; i < slices.left_length; i++)
  {
    char c = *(slices.left_start + i);
    char buffer[2] = { c, '\0' };
    platform_draw_text(
      buffer, 
      x_at[i], 
      y_at[i], 
      width_at[i], 
      height_at[i]);
  }

  // cleanup
  free(x_at);
  free(y_at);
  free(width_at);
  free(height_at);
}
