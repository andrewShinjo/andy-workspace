#ifndef GAP_BUFFER_T_H
#define GAP_BUFFER_T_H

#include <stddef.h>

typedef struct 
{
  char *data;
  size_t capacity;
  size_t gap_start;
  size_t gap_end;
} gap_buffer_t;

typedef struct
{
  char *left_start;
  size_t left_length;
  char *right_start;
  size_t right_length;
} gap_buffer_slice_t;

// Init and cleanup.

void gap_buffer_init(gap_buffer_t *self, size_t capacity);

void gap_buffer_cleanup(gap_buffer_t *self);

// Text

void gap_buffer_delete_text(gap_buffer_t *self, size_t count);

gap_buffer_slice_t gap_buffer_get_slices(gap_buffer_t *self);

size_t gap_buffer_get_text_size(gap_buffer_t *self);

void gap_buffer_insert_text(gap_buffer_t *self, 
                            char *text, 
                            size_t length);

// Buffer

size_t gap_buffer_get_capacity(gap_buffer_t *self);

void gap_buffer_resize(gap_buffer_t *self, size_t new_capacity);

// Gap

size_t gap_buffer_get_gap_size(gap_buffer_t *self);

void gap_buffer_set_gap_start(gap_buffer_t *self, size_t new_gap_start);

#endif
