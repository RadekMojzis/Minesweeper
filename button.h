
#ifndef MY_SDL_BUTTON
#define MY_SDL_BUTTON

#include <SDL.h>
#include <SDL_image.h>

const int M_HOVER = 0;
const int M_OUT = 1;
const int M_DOWN = 2;

class button{
  private:
    SDL_Rect position;
		int displace_x;
		int displace_y;
    
    SDL_Rect original_clip;
    SDL_Rect current_clip;
    
    int state = M_OUT;
    int func_number;
    void identify(int number);
  public:
	  int handle_mouse(SDL_Event event);
    void adjust_position();
    void render();
    void changestate(int newstate);
    button(SDL_Rect position, SDL_Rect clip, int foo);
};

#endif