
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "game.h"
#include <stdio.h>

extern SDL_Renderer* main_renderer;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern FILE* out;


int options(){
  fprintf(out, "Options - 3\n");

  return 0;
}