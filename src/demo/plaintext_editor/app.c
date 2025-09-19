#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../lib/data_structures/gap_buffer/gap_buffer_t.h"
#include "../../platform/platform.h"
#include "app.h"

#define private_function static

void app_update_and_render(app_t *app)
{
  gap_buffer_t *gap_buffer = app->gap_buffer;
  rectangle_t rectangle = app->rectangle;
  gap_buffer_slice_t slices = gap_buffer_get_slices(gap_buffer);
  char *event_text = app->event_text;

  // Update gap_buffer.
  if(app->event_text[0] != '\0')
  {
    gap_buffer_insert_text(
      gap_buffer,
      app->event_text,
      strlen(app->event_text)
    );
  }

  size_t text_length = gap_buffer_get_text_length(gap_buffer);
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

  for(int i = 0; i < slices.right_length; i++)
  {
    char c = slices.left_start[i];
    int char_height = platform_get_text_height(&c, 1);
    int char_width = platform_get_text_width(c, 1);
    if(current_x + char_width > rectangle.x + rectangle.width)
    {
      current_x = rectangle.x;
      current_y += char_height;
    }
    x_at[slices.left_length + i] = current_x;
    y_at[slices.left_length + i] = current_y;
    width_at[slices.left_length + i] = char_width;
    height_at[slices.left_length + i] = char_height;
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

  // Draw the text cursor.
  platform_fill_rectangle(
    x_at[slices.left_length - 1] + width_at[slices.left_length - 1],
    y_at[slices.left_length - 1],
    2,
    64
  );

  for(int i = 0; i < slices.right_length; i++)
  {
    char c = slices.right_start[i];
    char buffer[2] = { c, '\0' };
    platform_draw_text(
      buffer,
      x_at[slices.left_length + i],
      y_at[slices.left_length + i],
      width_at[slices.left_length + i],
      height_at[slices.left_length + i]
    );
  }

  // cleanup
  free(x_at);
  free(y_at);
  free(width_at);
  free(height_at);
  app->event_text[0] = '\0';
}
