
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "print_number.h"

SDL_Rect clips[26] = {
  {300, 50 , 50, 50}, //0
  {350, 50 , 50, 50}, //1
  {400, 50 , 50, 50}, //2
  {450, 50 , 50, 50}, //3
  {0, 100 , 50, 50}, //4
  {50, 100 , 50, 50}, //5
  {100, 100 , 50, 50}, //6
  {150, 100 , 50, 50}, //7
  {200, 100 , 50, 50}, //8
  {250, 100 , 50, 50}, //9
  {300, 100 , 50, 50} //:
};

extern SDL_Texture *bg_texture;
extern SDL_Texture *number_texture;
extern SDL_Renderer *main_renderer;

void make_number(int number, int off_x, int off_y, int font_size){
  if(number < 0)
    return;

  int backup = number;
  int numberlen = 1;
  
  while(number / 10){
    number /= 10;
    numberlen++;
  }
  number = backup;
  SDL_Rect bgclip = {0, 0, 3*font_size, font_size};
  SDL_Rect dstclip = {off_x, off_y, font_size, font_size};
  
  SDL_RenderCopy(main_renderer, bg_texture, &bgclip, &dstclip );
  
  for(int i = 0; i < numberlen; i++){
    dstclip = {off_x+(numberlen-i-1)*font_size, off_y, font_size, font_size};
    SDL_RenderCopy(main_renderer, number_texture, &clips[number%10], &dstclip);
    number -= number %10;
    number /= 10;
  }
  return;
}

void maketime(int number, int off_x, int off_y, int font_size){
  make_number(number/60, off_x, off_y, font_size);
  int backup = number;
  number /= 60;
  int numberlen = 1;
  while(number / 10){
    number /= 10;
    numberlen++;
  }
  number = backup;
  SDL_Rect dstclip = {off_x + (numberlen)*font_size, off_y, font_size, font_size};
  SDL_RenderCopy(main_renderer, number_texture, &clips[10], &dstclip);
  make_number(number%60, (numberlen+1)*font_size + off_x, off_y, font_size);
}
