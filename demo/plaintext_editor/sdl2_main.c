#include <SDL.h>
#include <stdio.h>

#include "../../lib/draw/lib_draw.h"
#include "../../lib/data_structures/gap_buffer/gap_buffer_t.h"
#include "app.h"

#include "../../lib/data_structures/gap_buffer/gap_buffer_t.c"
#include "app.c"

int main()
{
  printf("plaintext_editor SDL2\n");

  if(SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
    "plaintext_editor SDL2",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    800,
    600,
    SDL_WINDOW_SHOWN);

  if(!window)
  {
    SDL_Log("Failed to create a window: %s", SDL_GetError());
    return 1;
  }

  SDL_RaiseWindow(window);

  // Application state

  gap_buffer_t gap_buffer = {};
  gap_buffer_init(&gap_buffer, 64);
  gap_buffer_insert_text(&gap_buffer, "A", 1);

  int width, height;
  SDL_GetWindowSize(window, &width, &height);

  rectangle_t rectangle = {
    .x = 0,
    .y = 0,
    .width = width,
    .height = height
  };

  app_t app = {
    .gap_buffer = &gap_buffer,
    .rectangle = rectangle
  };

  SDL_Event event;
  int running = 1;

  while(running)
  {
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT)
      {
        running = 0;
      }
    }

    // update();
    app_render(&app);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
