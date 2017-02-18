
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "game.h"

extern SDL_Renderer* main_renderer;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern FILE* out;

int new_game(){
  SDL_SetRenderDrawColor( main_renderer, 0xF0, 0xF0, 0xF0, 0xFF );
	SDL_RenderClear( main_renderer );
  SDL_RenderPresent( main_renderer );
  
  SDL_Delay(2000);
  fprintf(out, "New game - 1\n");
  return 0;
}