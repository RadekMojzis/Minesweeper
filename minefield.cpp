

#include "minefield.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "bot.h"
SDL_Rect mineclip_array[15] = {
    //this part is for the skin.png
  {0, 0, 38, 38},  //hover
  {38, 0, 38, 38}, //nothing
  {0, 38, 38, 38}, //down
  {38, 38, 38, 38}, //bomb

  {76, 0, 38, 38}, //1
  {114, 0, 38, 38}, //2
  {76, 38, 38, 38}, //3
  {114, 38, 38, 38}, //4

  {76, 76, 38, 38}, // 5
  {114, 76, 38, 38}, //6
  {38, 76, 38, 38}, // 7
  {0, 76 , 38, 38}, // 8

  {152, 0, 38, 38}, //hoverflag 12
  {152, 38 , 38, 38},//flag      13
  {152, 76 , 38, 38}//flag false 14
  //this is for font
};

int current_tilesize = 38;

extern SDL_Texture* tile_texture;
extern SDL_Renderer* main_renderer;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern FILE* out;

mine_tile* last_active = NULL;
mine_tile* active = NULL;


void mine_tile::set_tile(SDL_Rect poss){
  position = poss;
  mineflag = 0;
  flagflag = 0;
  revealedflag = 0;
  mines_around = 0;
  clip = {38, 0, current_tilesize, current_tilesize};
}

int lossflag = 0;

void mine_tile::reveal(){
  if(revealedflag)
    return;
  if(flagflag && !lossflag)
    return;
  revealedflag = true;
  if(mineflag){
    clip = mineclip_array[3];
    revealedflag = true;
    render();
    lossflag = 1;
    return;
  }
  clip = mines_around ? mineclip_array[mines_around+3] : mineclip_array[M_DOWN];
  render();
}
/*
void mine_tile::bot_reveal(){
  revealedflag = true;
}
*/

void mine_tile::change_state(int new_state){
  if(new_state == M_FALSE){
    clip = mineclip_array[14];
    render();
    return;
  }
  if(revealedflag)
    return;
  if(new_state == M_HOVER){
    if(flagflag){
      clip = mineclip_array[12];
      render();
      return;
    }
    clip = mineclip_array[0];
    render();
  }
  if(new_state == M_DOWN){  
    if(flagflag){
      clip = mineclip_array[12];
      render();
      return;
    }
    clip = mineclip_array[2];
    render();
  }
  if(new_state == M_OUT){
    if(flagflag){
      clip = mineclip_array[13];
      render();
      return;
    }
    clip = mineclip_array[1];
    render();
  }
  
}

void mine_tile::adjust_position(SDL_Rect poss){
  position = poss;
  render();
}

void mine_tile::toggle_flag(){
  if(revealedflag)return;
  flagflag = flagflag == 0 ? 1 : 0;
  change_state(M_HOVER);
}


void mine_tile::render(){
  //fprintf(out, "Rendering @ %i:%i size %i x %i;\n", position.x, position.y, position.w, position.h);
  SDL_RenderCopy(main_renderer, tile_texture, &clip, &position);
}


minefield::minefield(int size_x, int size_y, int minecount){
  position.x = SCREEN_WIDTH/2-tiles_x*38/2;
  position.y = SCREEN_HEIGHT/2-tiles_y*38/2;
  position.w = size_x * 38;
  position.h = size_y * 38;
  tiles_x = size_x;
  tiles_y = size_y;
  n_o_mines = minecount;
  genflag = false;
  tiles = new mine_tile*[size_x];
  for(int i = 0; i < size_x; i++)
    tiles[i] = new mine_tile[size_y];
  
  for(int i = 0; i < size_x; i++){
    for(int j = 0; j < size_y; j++){
      tiles[i][j].set_tile({position.x + i*38 , position.y + j*38,38,38});
    }
  }
  adjust_position();
}

minefield::~minefield(){
  for(int i = 0; i < tiles_x; i++)
    delete [] tiles[i];
  delete [] tiles;
}

void minefield::render(){
  for(int i = 0; i < tiles_x; i++){
    for(int j = 0; j < tiles_y; j++){
      tiles[i][j].render();
    }
  }
  SDL_RenderPresent( main_renderer );
}

void minefield::adjust_position(){
  
  if(SCREEN_WIDTH > 100){
      current_tilesize = (SCREEN_WIDTH-100)/tiles_x;
  }
  if(SCREEN_HEIGHT > 100){
      current_tilesize = (SCREEN_HEIGHT-100)/tiles_y > current_tilesize ? current_tilesize :  (SCREEN_HEIGHT-100)/tiles_y ;
  }
  if(current_tilesize < 20){
    current_tilesize = (SCREEN_WIDTH-5)/tiles_x;
    current_tilesize = (SCREEN_HEIGHT-5)/tiles_y > current_tilesize ? current_tilesize :  (SCREEN_HEIGHT-5)/tiles_y;
  }
  
  position.x = SCREEN_WIDTH/2-tiles_x*current_tilesize/2;
  position.y = SCREEN_HEIGHT/2-tiles_y*current_tilesize/2;
  position.w = tiles_x * current_tilesize;
  position.h = tiles_y * current_tilesize;
  
  for(int i = 0; i < tiles_x; i++){
    for(int j = 0; j < tiles_y; j++){
      tiles[i][j].adjust_position({position.x + i*current_tilesize , position.y + j*current_tilesize,current_tilesize,current_tilesize});
    }
  }
}

void minefield::generate_mines(int count, int x, int y){
  genflag = true;
  if(x > 0 && y > 0)                 tiles[x-1][y-1].starting_ground = 1;
  if(y > 0)                          tiles[x][y-1].starting_ground = 1;
  if(x < tiles_x-1 && y > 0)         tiles[x+1][y-1].starting_ground = 1;

  if(x > 0)                          tiles[x-1][y].starting_ground = 1;
  if(true)                           tiles[x][y].starting_ground = 1;
  if(x < tiles_x-1)                  tiles[x+1][y].starting_ground = 1;

  if(x > 0 && y < tiles_y-1)         tiles[x-1][y+1].starting_ground = 1;
  if(y < tiles_y-1)                  tiles[x][y+1].starting_ground = 1;
  if(x < tiles_x-1 && y < tiles_y-1) tiles[x+1][y+1].starting_ground = 1;
  
  srand (time(NULL));
  int mines = 0;
  int stack_limit = 100000; //
  while( mines < count){
    for(int i = 0; i < tiles_x; i++){
      for(int j = 0; j < tiles_y; j++){
        if(((rand() % 250) == 0) && !tiles[i][j].mineflag && !tiles[i][j].starting_ground){
          if(mines == count)
            break;
          tiles[i][j].mineflag = 1; 
          mines++;
        }
      }
    }
    if(!stack_limit--)
      break;
  }
  for(int i = 0; i < tiles_x; i++){
    for(int j = 0; j < tiles_y; j++){
      if(tiles[i][j].mineflag){
        if(i > 0 && j > 0)                 tiles[i-1][j-1].mines_around++;
        if(j > 0)                          tiles[i][j-1].mines_around++;
        if(i < tiles_x-1 && j > 0)         tiles[i+1][j-1].mines_around++;

        if(i > 0)                          tiles[i-1][j].mines_around++;
        if(true)                           tiles[i][j].mines_around++;
        if(i < tiles_x-1)                  tiles[i+1][j].mines_around++;

        if(i > 0 && j < tiles_y-1)         tiles[i-1][j+1].mines_around++;
        if(j < tiles_y-1)                  tiles[i][j+1].mines_around++;
        if(i < tiles_x-1 && j < tiles_y-1) tiles[i+1][j+1].mines_around++;
      }
    }
  }
  gametime.start();
}

void minefield::gettile(SDL_Event event){
  int x = (event.motion.x-position.x)/current_tilesize;
  int y = (event.motion.y-position.y)/current_tilesize;
  if(last_active != &tiles[x][y] && last_active != NULL){
    last_active->change_state(M_OUT);
  }
  last_active = active;
  active = &tiles[x][y];
}

bool minefield::checkwin(){
  bool winflag = true;
  for(int i = 0; i < tiles_x; i++){
    for(int j = 0; j < tiles_y; j++){
      if(!tiles[i][j].revealedflag && !tiles[i][j].flagflag){
        winflag = false;
      }
      if(tiles[i][j].flagflag && !tiles[i][j].mineflag){
        winflag = false;
      }
    }
  }
  return winflag;
}

void minefield::oracle(int x, int y){
  if(x < 0 || y < 0 || x >= tiles_x || y >= tiles_y)
    return;
  if(tiles[x][y].flagflag)
    return;
  if(tiles[x][y].revealedflag)
    return;
  if(tiles[x][y].mines_around){
    tiles[x][y].reveal();
    return;
  }
  if(tiles[x][y].mines_around == 0){
    if(tiles[x][y].mineflag){return;}
    tiles[x][y].reveal();
    if(x > 0 && y > 0)                oracle(x-1, y -1);
    if(y > 0)                         oracle(x, y -1);
    if(x < tiles_x-1 && y > 0)        oracle(x+1, y -1);

    if(x > 0)                         oracle(x -1, y);
    if(x < tiles_x-1)                 oracle(x +1, y);

    if(x > 0 && y < tiles_y-1)        oracle(x-1, y+1);
    if(y < tiles_y-1)                 oracle(x, y+1);
    if(x < tiles_x-1 && y < tiles_y-1)oracle(x+1, y+1);
  }
  return;
}
/*
void minefield::bot_oracle(int x, int y){
  if(x < 0 || y < 0 || x >= tiles_x || y >= tiles_y)
    return;
  if(tiles[x][y].flagflag)
    return;
  if(tiles[x][y].revealedflag)
    return;
  if(tiles[x][y].mines_around){
    tiles[x][y].bot_reveal();
    return;
  }
  if(tiles[x][y].mines_around == 0){
    if(tiles[x][y].mineflag){return;}
    tiles[x][y].bot_reveal();
    if(x > 0 && y > 0)                bot_oracle(x-1, y -1);
    if(y > 0)                         bot_oracle(x, y -1);
    if(x < tiles_x-1 && y > 0)        bot_oracle(x+1, y -1);

    if(x > 0)                         bot_oracle(x -1, y);
    if(x < tiles_x-1)                 bot_oracle(x +1, y);

    if(x > 0 && y < tiles_y-1)        bot_oracle(x-1, y+1);
    if(y < tiles_y-1)                 bot_oracle(x, y+1);
    if(x < tiles_x-1 && y < tiles_y-1)bot_oracle(x+1, y+1);
  }
  return;
}
*/
/*
void minefield::bot_reveal_near(int x, int y){
  if(x < 0 || y < 0 || x >= tiles_x || y >= tiles_y)
    return;

  int adj_flags = 0;
  if(x > 0 && y > 0 && tiles[x-1][y-1].flagflag)                  adj_flags++;
  if(y > 0 && tiles[x][y-1].flagflag)                             adj_flags++;
  if(x < tiles_x-1 && y > 0 && tiles[x+1][y-1].flagflag)          adj_flags++;

  if(x > 0 && tiles[x-1][y].flagflag)                             adj_flags++;
  if(x < tiles_x-1 && tiles[x+1][y].flagflag)                     adj_flags++;

  if(x > 0 && y < tiles_y-1 && tiles[x-1][y+1].flagflag)          adj_flags++;
  if(y < tiles_y-1 && tiles[x][y+1].flagflag)                     adj_flags++;
  if(x < tiles_x-1 && y < tiles_y-1 && tiles[x+1][y+1].flagflag)  adj_flags++;

  if(adj_flags != tiles[x][y].mines_around)
    return;

  if(!tiles[x][y].revealedflag)
    return;
  if(x > 0 && y > 0){
    if(tiles[x-1][y-1].mines_around == 0){
      bot_oracle(x-1, y-1);}
    else{ tiles[x-1][y-1].bot_reveal();}
  }
  if(y > 0){
    if(tiles[x][y-1].mines_around == 0){
      bot_oracle(x, y-1);}
    else{tiles[x][y-1].bot_reveal();}
  }

  if(x < tiles_x-1 && y > 0) {
    if(tiles[x+1][y-1].mines_around == 0){
      bot_oracle(x+1, y-1);}
    else{tiles[x+1][y-1].bot_reveal();}
  }

  if(x > 0) {
    if(tiles[x-1][y].mines_around == 0){
      bot_oracle(x-1, y);}
    else{tiles[x-1][y].bot_reveal();}
  }
  if(x < tiles_x-1){
    if(tiles[x+1][y].mines_around == 0){
      bot_oracle(x+1, y);}
    else{tiles[x+1][y].bot_reveal();}
  }

  if(x > 0 && y < tiles_y-1){
    if(tiles[x-1][y+1].mines_around == 0){
      bot_oracle(x-1, y+1);}
    else{tiles[x-1][y+1].bot_reveal();}
  }
  if(y < tiles_y-1){
    if(tiles[x][y+1].mines_around == 0){
      bot_oracle(x, y+1);}
    else{tiles[x][y+1].bot_reveal();}
  }
  if(x < tiles_x-1 && y < tiles_y-1){
    if(tiles[x+1][y+1].mines_around == 0){
      bot_oracle(x+1, y+1);}
    else{tiles[x+1][y+1].bot_reveal();}
  }
}
*/
void minefield::reveal_near(int x, int y){
  if(x < 0 || y < 0 || x >= tiles_x || y >= tiles_y)
    return;

  int adj_flags = 0;
  if(x > 0 && y > 0 && tiles[x-1][y-1].flagflag)                  adj_flags++;
  if(y > 0 && tiles[x][y-1].flagflag)                             adj_flags++;
  if(x < tiles_x-1 && y > 0 && tiles[x+1][y-1].flagflag)          adj_flags++;

  if(x > 0 && tiles[x-1][y].flagflag)                             adj_flags++;
  if(x < tiles_x-1 && tiles[x+1][y].flagflag)                     adj_flags++;

  if(x > 0 && y < tiles_y-1 && tiles[x-1][y+1].flagflag)          adj_flags++;
  if(y < tiles_y-1 && tiles[x][y+1].flagflag)                     adj_flags++;
  if(x < tiles_x-1 && y < tiles_y-1 && tiles[x+1][y+1].flagflag)  adj_flags++;

  if(adj_flags != tiles[x][y].mines_around)
    return;

  if(!tiles[x][y].revealedflag)
    return;
  if(x > 0 && y > 0){
    if(tiles[x-1][y-1].mines_around == 0){
      oracle(x-1, y-1);}
    else{ tiles[x-1][y-1].reveal();}
  }
  if(y > 0){
    if(tiles[x][y-1].mines_around == 0){
      oracle(x, y-1);}
    else{tiles[x][y-1].reveal();}
  }

  if(x < tiles_x-1 && y > 0) {
    if(tiles[x+1][y-1].mines_around == 0){
      oracle(x+1, y-1);}
    else{tiles[x+1][y-1].reveal();}
  }

  if(x > 0) {
    if(tiles[x-1][y].mines_around == 0){
      oracle(x-1, y);}
    else{tiles[x-1][y].reveal();}
  }
  if(x < tiles_x-1){
    if(tiles[x+1][y].mines_around == 0){
      oracle(x+1, y);}
    else{tiles[x+1][y].reveal();}
  }

  if(x > 0 && y < tiles_y-1){
    if(tiles[x-1][y+1].mines_around == 0){
      oracle(x-1, y+1);}
    else{tiles[x-1][y+1].reveal();}
  }
  if(y < tiles_y-1){
    if(tiles[x][y+1].mines_around == 0){
      oracle(x, y+1);}
    else{tiles[x][y+1].reveal();}
  }
  if(x < tiles_x-1 && y < tiles_y-1){
    if(tiles[x+1][y+1].mines_around == 0){
      oracle(x+1, y+1);}
    else{tiles[x+1][y+1].reveal();}
  }
}

int minefield::refresh(){
	for(int i = 0; i < tiles_x; i++)
		for(int j = 0; j < tiles_y; j++)
			tiles[i][j].change_state(M_OUT);
}

void minefield::reveal(){
  for(int i = 0; i < tiles_x; i++){
    for(int j =0; j < tiles_y; j++){
      if(tiles[i][j].flagflag && !tiles[i][j].mineflag){
        tiles[i][j].change_state(M_FALSE);
      }
      if(!tiles[i][j].flagflag && tiles[i][j].mineflag){
        tiles[i][j].reveal();
      }
    }
  }
  lossflag = 0;
  SDL_RenderPresent( main_renderer );
  SDL_Event event;
  bool stopflag = true;
  while(stopflag){
    if(SDL_PollEvent( &event )){
      if(event.type == SDL_QUIT)
        return;
      if(event.type == SDL_MOUSEBUTTONUP)
        stopflag = false;
    }
    SDL_Delay(50);
  }
  return;
}

int minefield::handle_mouse(SDL_Event event){
  switch (event.type)  {
    case SDL_WINDOWEVENT:  {
      if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)  {
        SCREEN_WIDTH = event.window.data1;
        SCREEN_HEIGHT = event.window.data2;
        SDL_RenderClear( main_renderer );
        adjust_position();
        render();
      }
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      if( event.button.button == SDL_BUTTON_RIGHT )
        return 0;
      if(event.motion.x >= position.x && event.motion.x <= position.x + position.w
			&& event.motion.y >= position.y && event.motion.y <= position.y + position.h){
			  gettile(event);
        active->change_state(M_DOWN);
      }
      break;
    }
    case SDL_MOUSEMOTION: { 
      if(event.motion.x > position.x && event.motion.x < position.x + position.w
			&& event.motion.y > position.y && event.motion.y < position.y + position.h){
			  gettile(event);
        if(event.button.button == SDL_BUTTON_LEFT){
          active->change_state(M_DOWN);}
        else{
          active->change_state(M_HOVER);}

      }
      else if(last_active != NULL){
        last_active->change_state(M_OUT);
        last_active = NULL;
      }
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      if(event.motion.x >= position.x && event.motion.x <= position.x + position.w
			&& event.motion.y >= position.y && event.motion.y <= position.y + position.h){
        if( event.button.button == SDL_BUTTON_RIGHT ){
          gettile(event);
          if(!active->revealedflag){
            flagcount += active->flagflag == 0 ? 1 : -1;
            SDL_RenderPresent( main_renderer );
          }
          active->toggle_flag();
          
          if(lossflag) return 2;
          if(checkwin())return 1;
        }
        if( event.button.button == SDL_BUTTON_LEFT ){
          if(!genflag){
            generate_mines(n_o_mines,(event.motion.x-position.x)/current_tilesize,(event.motion.y-position.y)/current_tilesize);
          }
          if(active->revealedflag){
            int i = 0;
            while(i < 20){
              if(SDL_PollEvent( &event )){
                if(event.type == SDL_MOUSEBUTTONUP){
                  if(event.button.button == SDL_BUTTON_LEFT){
                    reveal_near((event.motion.x-position.x)/current_tilesize,(event.motion.y-position.y)/current_tilesize);
                    if(lossflag) return 2;
                    if(checkwin())return 1;
                    break;
                  }
                }
              }
              i++;
              SDL_Delay(12);
            }
          }else{
            oracle((event.motion.x-position.x)/current_tilesize,(event.motion.y-position.y)/current_tilesize);
            if(lossflag) return 2;
            if(checkwin())return 1;
           }
        }
      }
      else{
        use_bot(this);
      }
      break;
    }
  }
  return 0;
}