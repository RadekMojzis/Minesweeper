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
		int danger = 0;
    
    int mines_around = 0;
    void reveal();
    //void bot_reveal();
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
  
  public:
    friend void use_bot(minefield* field);
    friend int put_flags_around(minefield * field, int i, int j);
    friend int n_o_unrevealed_tiles(minefield * field, int i, int j);
    friend int n_o_adjucent_flags(minefield * field, int i, int j);
    friend void eval_danger(minefield *field, int mines, int out_of);
		friend void set_danger(minefield *field, int i, int j);
		
    int n_o_mines;
    int flagcount = 0;
	  int refresh();
    Timer gametime;
    void reveal();
    void reveal_near(int x, int y);
    //void bot_reveal_near(int x, int y);
    void oracle(int x, int y);
    //void bot_oracle(int x, int y);
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