#ifndef PLATFORM_H
#define PLATFORM_H

#include <SDL.h>
#include <SDL_ttf.h>

typedef struct platform_font_t platform_font_t;
typedef void* platform_font_handle;

typedef struct platform_renderer_t platform_renderer_t;
typedef void* platform_renderer_handle;

extern platform_font_t font;
extern platform_renderer_t renderer;

/* Font */

void platform_set_font_handle(platform_font_handle font_handle);

/* Renderer */

void platform_set_renderer_handle(
  platform_renderer_handle renderer_handle);

/* Text */

void platform_draw_text(char *text, int x, int y, int width, int height);
int platform_get_text_height(char *text, int text_length);
int platform_get_text_width(char c, int text_length);

/* Draw */

void platform_fill_rectangle(int x, int y, int width, int height);

#endif

