
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

//Function declarations
SDL_Texture* loadTexture( std::string path );
int init();
void deinit();
bool load_menu();
void display_menu();
bool load_game();

//Global variables
SDL_Window* window = NULL;
SDL_Renderer* main_renderer = NULL;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Texture button_textures = NULL;

bool skip_menu_flag = false;

int main(int argc, char* args[]){
	if(init()) return 1;
	if(!skip_menu_flag) 
	  if(load_menu()) return 1;
	else
		if(load_game()) return 1;
	
	return 0;
}

bool init(){
	window = SDL_CreateWindow( "Minesweeper", 
														SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
														SCREEN_WIDTH, SCREEN_HEIGHT, 
														SDL_WINDOW_RESIZABLE );
														
  main_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(window == NULL || main_renderer == NULL) return true;
	return false;
}

void deinit(){
	SDL_DestroyRenderer(main_renderer);
	//SDL_DestroyTexture(button_texture);
  SDL_DestroyWindow( window );
	SDL_Quit();
}

bool load_menu(){
	button_textures  = loadTexture("menu.bmp");
	display_menu();
	return false;
}

void display_menu(){
	SDL_SetRenderDrawColor( main_renderer, 0xF0, 0xF0, 0xF0, 0xFF );
	SDL_RenderClear( main_renderer );
  
	
	SDL_RenderPresent( main_renderer );
	
	return;
}

bool load_game(){
	return false;
}

SDL_Texture* loadTexture( std::string path ){
  return SDL_CreateTextureFromSurface( main_renderer, IMG_Load( path.c_str() ));
}
