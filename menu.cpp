
#include "menu.h"
#include "button.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdio.h>
extern SDL_Renderer* main_renderer;
extern SDL_Texture* button_textures;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern FILE* out;


bool load_menu();
int display_menu();
int menu_handle_events(SDL_Event event);

button* existing_buttons[10000];
int buttoncount = 0;
SDL_Texture* button_textures = NULL;

int menu(int skip){
	if(skip)
    return 1;
  button_textures  = SDL_CreateTextureFromSurface( main_renderer, IMG_Load("menu.bmp"));
  
	int function_number = display_menu();
  SDL_DestroyTexture(button_textures);
  return function_number;
}

int display_menu(){
	SDL_SetRenderDrawColor( main_renderer, 0xF0, 0xF0, 0xF0, 0xFF );
	SDL_RenderClear( main_renderer );
  int spacing = 75;
  button NEW_GAME_BTN({0, -spacing, 150, 50},{0,0,300,100}, 1);
  existing_buttons[buttoncount++] = &NEW_GAME_BTN;
  button RECORDS_BTN({0, 0*spacing, 150, 50},{0,300,300,100}, 2);
  existing_buttons[buttoncount++] = &RECORDS_BTN;
  button OPTIONS_BTN({0, spacing, 150, 50},{0,100,300,100}, 3);
  existing_buttons[buttoncount++] = &OPTIONS_BTN;
  button QUIT_BTN({0, 2*spacing, 150, 50},{0,200,300,100}, 0);  
  existing_buttons[buttoncount++] = &QUIT_BTN;
  SDL_Event event;

  bool die = false;
  while( !die ){
	  while( SDL_PollEvent( &event ) != 0 ){
			if( event.type == SDL_QUIT ){
				die = true;
			}
      int func_number;
      if((func_number = menu_handle_events(event)))
        return func_number;
	  }
		SDL_RenderPresent( main_renderer );
		SDL_Delay(20);
	}
  
  for(int i = buttoncount-1; i >= 0; i--){
    existing_buttons[i] = NULL;
	}
  buttoncount = 0;

  return 0;
}

int menu_handle_events(SDL_Event event){
  
  switch (event.type)  {
    case SDL_WINDOWEVENT:  {
      switch (event.window.event)  {
        case SDL_WINDOWEVENT_SIZE_CHANGED:  {
          SCREEN_WIDTH = event.window.data1;
          SCREEN_HEIGHT = event.window.data2;
					SDL_RenderClear( main_renderer );
          for(int i = buttoncount-1; i >= 0; i--){
            existing_buttons[i]->adjust_position();
          }
          break;
        }
      }
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
			for(int i = buttoncount-1; i >= 0; i--){
        existing_buttons[i]->handle_mouse(event);
		  }
		}
    case SDL_MOUSEMOTION: {
			for(int i = buttoncount-1; i >= 0; i--){
        existing_buttons[i]->handle_mouse(event);
		  }
			break;
		}
    case SDL_MOUSEBUTTONUP: {
			for(int i = buttoncount-1; i >= 0; i--){
        int button_function;
        if((button_function = existing_buttons[i]->handle_mouse(event)))
          return button_function;
		  }
			break;
		}
  }
  return 0;
}
