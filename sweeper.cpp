
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "game.h"
#include "options.h"
#include "records.h"
#include "menu.h"

//Global variables - also used as externs in other files

SDL_Window* window = NULL;
SDL_Renderer* main_renderer = NULL;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

FILE* out;
int skip_menu_flag;

//Function declarations

bool init();
void deinit();
int Exit_game();

int main(int argc, char *argv[]){
	if(init()) return 1;
	
  int func_number = menu(skip_menu_flag);
  
  while(func_number != 0){
    if(func_number == 1)
      func_number = new_game();
    if(func_number == 2)
      func_number = records();
    if(func_number == 3)
      func_number = options();
    if(func_number == 4)
      func_number = menu();
  }
  Exit_game();
  
	return 0;
}

bool init(){
  out = fopen("out.txt", "a");
  
  FILE* options = fopen("options.txt", "r");
  int trash;
  int fullscreen;
  fscanf(options, "%i %i %i %i %i", &trash, &trash, &trash, &fullscreen, &skip_menu_flag );
  fclose(options);
  
  Uint32 windowflag = SDL_WINDOW_RESIZABLE;
  if(fullscreen)
    windowflag |= SDL_WINDOW_MAXIMIZED;
  
	window = SDL_CreateWindow( "Minesweeper", 
														SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
														SCREEN_WIDTH, SCREEN_HEIGHT, 
														windowflag );
	
  main_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(window == NULL || main_renderer == NULL) return true;
  
	return false;
}

void deinit(){
	SDL_DestroyRenderer(main_renderer);
  SDL_DestroyWindow(window);
	SDL_Quit();
  fclose(out);
}

int Exit_game(){  
  deinit();
  return 0;
}

