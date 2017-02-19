#ifndef MY_MINEFIELD
#define MY_MINEFIELD

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "timer.h"
#include "button.h"


class mine_tile{
  private:
    SDL_Rect position;
    SDL_Rect clip;
    
  public:
    int starting_ground = 0;
    int mineflag = 0;
    int flagflag = 0;
    int revealedflag = 0;
    
    int mines_around = 0;
    void reveal();
    void toggle_flag();
    void set_tile(SDL_Rect poss);
    void adjust_position(SDL_Rect new_poss);
    void change_state(int new_state);
    void rescale(int new_size, int x, int y);
    void render();
};


class minefield{
  private:
    SDL_Rect position;
    mine_tile** tiles;
    int tiles_x;
    int tiles_y;
    int genflag = 0;
    void rescale();
    int n_o_mines;
    
  public:
    Timer gametime;
    void reveal_near(int x, int y);
    void oracle(int x, int y);
    void generate_mines(int count, int x, int y);
	  int handle_mouse(SDL_Event event);
    void adjust_position();
    void render();
    void gettile(SDL_Event event);
    bool checkwin();
    minefield *copy(minefield *source);
    minefield(int size_x, int size_y, int minecount);
    ~minefield();
};

#endif