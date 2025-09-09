#include <SDL.h>
#include <stdio.h>
#include "app.h"

#include "app.c"

int main()
{
  printf("plaintext-editor SDL2\n");

  if(SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
    "plaintext-editor SDL2",
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
    // render();
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
