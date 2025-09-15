#include <SDL_ttf.h>
#include "platform.h"

TTF_Font *font;

int platform_get_text_height(char *text, int text_length)
{
  int width, height;
  TTF_SizeText(font, text, &width, &height);
  return height;
}

int platform_get_text_width(char *text, int text_length)
{
  int width, height;
  TTF_SizeText(font, text, &width, &height);
  return width;
}
