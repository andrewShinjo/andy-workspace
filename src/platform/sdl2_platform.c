#include <SDL.h>
#include <SDL_ttf.h>
#include "platform.h"

struct platform_font_t
{
  TTF_Font *font_handle;
};

struct platform_renderer_t
{
  SDL_Renderer *renderer_handle;
};

platform_font_t font;
platform_renderer_t renderer;

/* Font */

void platform_set_font_handle(platform_font_handle font_handle)
{
  font.font_handle = font_handle;
}

/* Renderer */

void platform_set_renderer_handle(platform_renderer_handle renderer_handle)
{
  renderer.renderer_handle = renderer_handle;
}

/* Text */

void platform_draw_text(char *text, int x, int y, int width, int height)
{
  SDL_Color black = { 0, 0, 0, 255 };
  SDL_Surface *surface = TTF_RenderUTF8_Blended(
    font.font_handle, 
    text, 
    black);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(
    renderer.renderer_handle, 
    surface);
  
  SDL_Rect destination_rectangle = { x, y, surface->w, surface->h };
  SDL_RenderCopy(
    renderer.renderer_handle, 
    texture, 
    NULL, 
    &destination_rectangle);
  SDL_FreeSurface(surface);
}

int platform_get_text_height(char *text, int text_length)
{
  int width, height;
  TTF_SizeText(font.font_handle, text, &width, &height);
  return height;
}

int platform_get_text_width(char c, int text_length)
{
  int min_x, max_x, min_y, max_y, advance;
  TTF_GlyphMetrics(font.font_handle, c, &min_x, &max_x, &min_y,
    &max_y, &advance);
  return advance;
}
