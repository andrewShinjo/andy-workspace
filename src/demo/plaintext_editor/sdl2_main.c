#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

#include "../../lib/data_structures/gap_buffer/gap_buffer_t.h"
#include "app.h"

#include "../../lib/data_structures/gap_buffer/gap_buffer_t.c"
#include "../../platform/sdl2_platform.c"
#include "app.c"

int main()
{
  printf("plaintext_editor SDL2\n");

  if(SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }
  if(TTF_Init() == -1)
  {
    SDL_Log("Unable to initialize SDL_ttf: %s", TTF_GetError());
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
    SDL_Quit();
    return 1;
  }

  SDL_RaiseWindow(window);

  // Initialize renderer.
  SDL_Renderer *renderer = SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED);

  if(!renderer)
  {
    SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
    return 1;
  }

  platform_set_renderer_handle(renderer);

  // Initialize font.
  TTF_Font *font =
    TTF_OpenFont(
      "../assets/Inconsolata/static/Inconsolata-Regular.ttf", 
      64);
  if(!font)
  {
    SDL_Log("TTF_OpenFont error: %s", TTF_GetError());
    return 1;
  }

  platform_set_font_handle(font);
                                     
  // Application state

  gap_buffer_t gap_buffer = {};
  gap_buffer_init(&gap_buffer, 64);
  gap_buffer_insert_text(&gap_buffer, "Andrew Shinjo", 13);
  gap_buffer_set_gap_start(&gap_buffer, 2);

  printf("blah\n");
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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    app_render(&app);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
