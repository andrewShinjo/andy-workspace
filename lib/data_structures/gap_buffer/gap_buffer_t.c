#include <stdlib.h>
#include <string.h>
#include "gap_buffer_t.h"

// Init and cleanup.

void gap_buffer_init(gap_buffer_t *self, size_t capacity)
{
  self->data = malloc(capacity);
  self->capacity = capacity;
  self->gap_start = 0;
  self->gap_end = capacity;
}

void gap_buffer_clean(gap_buffer_t *self)
{
  free(self->data);
}

// Text

void gap_buffer_delete_text(gap_buffer_t *self, size_t count)
{
  self->gap_start -= count;
}

gap_buffer_slice_t gap_buffer_get_slices(gap_buffer_t *self)
{
  char *left_start = self->data;
  size_t left_length = self->gap_start;
  char *right_start = self->data + self->gap_end;
  size_t right_length = self->capacity - self->gap_end;

  gap_buffer_slice_t result = {
    .left_start = left_start,
    .left_length = left_length,
    .right_start = right_start,
    .right_length = right_length
  };

  return result;
}

size_t gap_buffer_get_text_size(gap_buffer_t *self)
{
  size_t left_length = self->gap_start;
  size_t right_length = self->capacity - self->gap_end;
  size_t result = left_length + right_length;
  return result;
}

void gap_buffer_insert_text(gap_buffer_t *self,
                            char *text,
                            size_t length)
{
  memcpy(self->data + self->gap_start, text, length);
  self->gap_start += length;
}

// Buffer

size_t gap_buffer_get_capacity(gap_buffer_t *self)
{
  return self->capacity;
}

void gap_buffer_resize(gap_buffer_t *self, size_t new_capacity)
{
  if(new_capacity <= self->capacity)
  {
    return;
  }

  char *new_data = malloc(new_capacity);

  // copy left side of gap.
  memcpy(new_data, self->data, self->gap_start);

  // copy right side of gap.
  size_t right_length = self->capacity - self->gap_end;
  memcpy(new_data + (new_capacity - right_length), 
         self->data + self->gap_end, 
         right_length);

  free(self->data);

  self->data = new_data;
  self->capacity = new_capacity;
  self->gap_end = new_capacity - right_length;
}

// Gap

size_t gap_buffer_get_gap_size(gap_buffer_t *self)
{
  return self->gap_end - self->gap_start;
}

void gap_buffer_set_gap_start(gap_buffer_t *self, size_t new_gap_start)
{
  int displacement = new_gap_start - self->gap_start;
  size_t left_length = self->gap_start;
  size_t right_length = self->capacity - self->gap_end;

  if(displacement == 0)
  {
    return;
  }
  else if(displacement > 0)
  {
    if(displacement > right_length)
    {
      return;
    }

    memcpy(self->data + self->gap_start, 
           self->data + self->gap_end, 
           displacement);
  }
  else if(displacement < 0)
  {
    if(-displacement > left_length)
    {
      return;
    }
    memcpy(self->data + self->gap_end + displacement,
           self->data + self->gap_start + displacement,
           displacement);
  }

  self->gap_start += displacement;
  self->gap_end += displacement;
}

