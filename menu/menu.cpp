#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* window = NULL;
SDL_Renderer* main_renderer = NULL;

SDL_Texture* bg_texture = NULL;
SDL_Texture* button_texture = NULL;


int init(){
	if(SDL_Init( SDL_INIT_VIDEO ) < 0)
		return 1;
	
	window = SDL_CreateWindow( "Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );
	main_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	return 0;
}

SDL_Texture* loadTexture( std::string path ){
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface( main_renderer, IMG_Load( path.c_str() ));
  if( newTexture == NULL ){
	  printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}

	return newTexture;
}

int load(){
	bg_texture  = loadTexture("img/bg.bmp");
	
	button_texture  = loadTexture("texture.bmp");

	return 0;
}

void deinit(){

    SDL_DestroyWindow( window );
    SDL_DestroyTexture(bg_texture);
		SDL_DestroyTexture(button_texture);
    SDL_DestroyRenderer(main_renderer);
    SDL_Quit();
}

int main(int argc, char* args[]){
	int err_code = 0;
		
  if(init() || load())
  	cout <<"Could not load files!\n" << "Error code: ";

  SDL_Rect clip1;
  SDL_Rect clip2;
  SDL_Rect dst1;
  SDL_Rect dst2;
	
	clip1.x = 0;
	clip1.y = 0;
	clip1.w = 150;
	clip1.h = 50;
	
	clip2.x = 0;
	clip2.y = 50;
	clip2.w = 150;
	clip2.h = 50;
	
	dst1.x = SCREEN_WIDTH/2-75;
	dst1.y = SCREEN_HEIGHT/2-75;
	dst1.w = 150;
	dst1.h = 50;
	
	dst2.x = SCREEN_WIDTH/2-75;
	dst2.y = SCREEN_HEIGHT/2+25;
	dst2.w = 150;
	dst2.h = 50;
	
	SDL_SetRenderDrawColor( main_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( main_renderer );
	SDL_RenderCopy(main_renderer, bg_texture, NULL, NULL);
	SDL_RenderCopy(main_renderer, button_texture, &clip1, &dst1);
	SDL_RenderCopy(main_renderer, button_texture, &clip2, &dst2);
	
	SDL_RenderPresent( main_renderer );
	
	bool quit = false;

	SDL_Event e;
	while( !quit ){
	  while( SDL_PollEvent( &e ) != 0 ){
			if( e.type == SDL_QUIT ){
				quit = true;
			}
	  }
		
		SDL_RenderClear( main_renderer );
		SDL_RenderCopy(main_renderer, bg_texture, NULL, NULL);
		SDL_RenderCopy(main_renderer, button_texture, &clip1, &dst1);
		SDL_RenderCopy(main_renderer, button_texture, &clip2, &dst2);
		
		SDL_RenderPresent( main_renderer );
		SDL_Delay(20);
	}
	
	deinit();
  
  return 0;
}
