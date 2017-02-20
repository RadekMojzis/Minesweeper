
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "game.h"
#include "minefield.h"
#include "print_number.h"
#include "button.h"
#include <stdio.h>
extern SDL_Renderer* main_renderer;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern FILE* out;
SDL_Texture *bg_texture = NULL;
SDL_Texture *number_texture = NULL;
SDL_Texture* tile_texture = NULL;
extern SDL_Texture* button_textures;


int x_size;
int y_size;
int minecount;

void game_deinit();
void game_deinit(){
	SDL_DestroyTexture(tile_texture);
	SDL_DestroyTexture(number_texture);
	
	
}

int init_game(){
  SDL_SetRenderDrawColor( main_renderer, 0xF0, 0xF0, 0xF0, 0xFF );
	SDL_RenderClear( main_renderer );
  SDL_RenderPresent( main_renderer );

  FILE* options = fopen("options.txt", "r");

  fscanf(options, "%i %i %i", &x_size, &y_size, &minecount);
  fclose(options);
  tile_texture  = SDL_CreateTextureFromSurface( main_renderer, IMG_Load("skin.bmp"));
  number_texture  = SDL_CreateTextureFromSurface( main_renderer, IMG_Load("font.bmp"));
  bg_texture  = SDL_CreateTextureFromSurface( main_renderer, IMG_Load("background.bmp"));
	
  if(tile_texture == NULL)
    fprintf(out, "TEXTURE MISSING\n");
  return 0;
}

int victory(){
  SDL_Event event;
  SDL_Texture* victory  = SDL_CreateTextureFromSurface( main_renderer, IMG_Load("success.bmp"));
  SDL_Texture* score  = SDL_CreateTextureFromSurface( main_renderer, IMG_Load("scoreboard.bmp"));
  button_textures  = SDL_CreateTextureFromSurface( main_renderer, IMG_Load("menu.bmp"));
  
  SDL_Rect victory_poss = {SCREEN_WIDTH/2-640/2, SCREEN_HEIGHT/2-150, 641, 192};
  SDL_RenderCopy(main_renderer, victory,NULL, &victory_poss);
  SDL_Rect score_poss = {SCREEN_WIDTH/2-500/2, SCREEN_HEIGHT/2+42, 500, 100};
  SDL_RenderCopy(main_renderer, score,NULL, &score_poss);
  
  button new_game( {-170,200, 300, 100},{0,0,300,100}, 1);
  button main_menu( {170,200, 300, 100}, {0,400,300,100}, 4);
  
  /*SDL_Rect ahoj = {SCREEN_WIDTH/2-640/2,SCREEN_HEIGHT/2+142, 300, 100};
  SDL_Rect ahoj2 = {0,0,300,100};
  
  SDL_RenderCopy(main_renderer, button_textures,&ahoj2 , &ahoj);
  */
  
  new_game.render();
  main_menu.render();
  SDL_RenderPresent( main_renderer );
  
  bool die2 = false;
  while(!die2){
    while( SDL_PollEvent( &event ) != 0 ){
      if( event.type == SDL_QUIT ){
        die2 = true;
      }
			int retval;
      if((retval = new_game.handle_mouse(event)))return retval;
      if((retval = main_menu.handle_mouse(event)))return retval;
    }
    SDL_RenderPresent( main_renderer );
    SDL_Delay(10);
  }
  return 0;
}

int new_game(){
  init_game();
  minefield field(x_size, y_size, minecount);
	SDL_RenderClear(main_renderer);
  field.refresh();
	
  SDL_RenderPresent( main_renderer );
  SDL_Event event;
  bool die = false;
  while( !die ){
    
	  while( SDL_PollEvent( &event ) != 0 ){
			if( event.type == SDL_QUIT ){
				die = true;
			}
      int retval;
      if((retval=field.handle_mouse(event)) == 1){
        field.gametime.pause();
        return victory();
      }
      if(retval == 2){
        fprintf(out, "Loss!\n");
      }
    }
    maketime(field.gametime.get_ticks()/1000, SCREEN_WIDTH/2-200, 0, 30);
    SDL_RenderPresent( main_renderer );
    SDL_Delay(10);
	}
  game_deinit();
  
  return 0;
}