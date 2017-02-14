#include <iostream>
#include <SDL.h>


using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* g_window = NULL;

SDL_Surface* surface = NULL;

SDL_Surface* immage = NULL;

int init(){
	if(SDL_Init( SDL_INIT_VIDEO ) < 0)
		return 1;
	
	g_window = SDL_CreateWindow( "Cosi...", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	surface = SDL_GetWindowSurface( g_window );
	
	return 0;
}

int load(){
	if((immage = SDL_LoadBMP( "img/Boom.bmp" )) == NULL)
		return 1;
	return 0;
}

void deinit(){
    SDL_FreeSurface( immage );

    SDL_DestroyWindow( g_window );

    SDL_Quit();
}

int main(int argc, char* args[]){
	int err_code = 0;
	
	
  if((err_code=init())){
		cout <<"Initialisation failed!\n" << "Error code: " << err_code;
		return 1;
  }
  
	if(err_code = load()){
		cout <<"Could not load files!\n" << "Error code: " << err_code;
  }

	
	SDL_BlitSurface( immage, NULL, surface, NULL );
	SDL_UpdateWindowSurface( g_window );
	
	SDL_Delay( 5000 );
	
	deinit();
  
  return 0;
}
