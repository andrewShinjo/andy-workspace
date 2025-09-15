#ifndef PLATFORM_H
#define PLATFORM_H

/* Text */

extern TTF_Font *font;

int platform_get_text_height(char *text, int text_length);

int platform_get_text_width(char *text, int text_length);


#endif

