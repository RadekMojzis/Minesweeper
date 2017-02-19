
#include <string>


#include "timer.h"
#include "sdl_must.h"


SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *buttonClip = NULL;
SDL_Surface *numberClip = NULL;
SDL_Surface *nuke = NULL;
SDL_Surface *success = NULL;
SDL_Surface *counter = NULL;
SDL_Surface *clockimg = NULL;
SDL_Surface *scoreboard = NULL;

SDL_Event event;

bool generated = false;  //the minefield generates after you click on the field so... this is for that
int gameover = 0;        //win or lose... game ends

const int FRAMES_PER_SECOND = 50; //framerate... if you dont capp it... shit happens 25% cpu gone :D

int minemax = 99;  //number of mines
int size_x = 30; //size of the field
int size_y = 15;

int flagcounter = minemax;  //this is the counter of mines-number of flags...

//Window properities
int SCREEN_WIDTH = 1200;
int SCREEN_HEIGHT = 670;
const int SCREEN_BPP = 32;
//constants used to cut the picture in pieces...
const int M_HOVER = 0;
const int M_OUT = 1;
const int M_DOWN = 2;
const int M_FALSE = 14;

SDL_Rect clips[26] = {
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
  {152, 76 , 38, 38},//flag false 14
  //this is for font

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


//ye the mine field is a 2D array of these
class button{
    private:
      SDL_Rect box;
      SDL_Rect* clip;
      int minecount;
      bool mine;
      int state;
      bool revealed;
      bool flag;
      bool gen;
    public:
      button();
      void reveal();
      void show();

      void plantmine();
      void addmine();
      void changeflag();
      void changestate(int newstate);
      void setbutton(int x, int y, int w, int h);
      void setgen();

      int getminecount();

      bool isavaileable();
      bool ismine();
      bool isrevealed();
      bool isflag();
};


void reveal_near(button **field);

bool button::isflag(){
  return flag;
}

bool button::isrevealed(){
  return revealed;
}

int button::getminecount(){
  return minecount;
}

bool button::isavaileable(){
  return !gen;
}

void button::setgen(){
  gen = true;
}

void button::changeflag(){
  if(revealed)
    return;
  flag = flag ? false : true;
  clip = flag? &clips[12] : &clips[M_HOVER];
  show();
}

void button::reveal(){
  if(revealed)
    return;
  if(flag)
    return;
  revealed = true;
  if(mine){
    clip = &clips[3];
    revealed = true;
    show();
    gameover = 1;  //endgame mechanics

    return;
  }
  clip = minecount ? &clips[minecount+3] : &clips[M_DOWN];
  show();
}

button::button(){
  box.x = 0;  //offset
  box.y = 0;
  box.w = 0;  //size
  box.h = 0;
  mine = false;
  flag = false;
  revealed = false;
  minecount = 0;
  gen = false;
  clip = &clips[M_OUT]; //clip
}

void button::setbutton(int x, int y, int w, int h){  //instead of constructor... because... one does not simply inicialise a 2D array of objects with creator..
  box.x = x;  //offset
  box.y = y;
  box.w = w;  //size
  box.h = h;
}

void button::plantmine(){
  mine = true;
}

bool button::ismine(){
  return mine;
}

void button::addmine(){
  minecount++;
}

void button::show(){
  apply_surface( box.x, box.y, buttonClip, screen, clip);
}

void button::changestate(int newstate){
  if(revealed)
    return;
  if(flag){
    if(newstate == M_HOVER){
      clip = &clips[12];
      show();
    }
    if(newstate == M_DOWN){
      clip = &clips[13];
      show();
    }
    if(newstate == M_OUT){
      clip = &clips[13];
      show();
    }
    if(newstate == M_FALSE){
      clip = &clips[M_FALSE];
      show();
    }
    return;
  }
  clip = &clips[newstate];
  show();
}

button *last_active = NULL;  // THIS SUCKER IS HIDDEN - stores the last active (the button mouse have been before the current active)

int init(){ //inicializes SDL stuff
  FILE *fr;
  if((fr = fopen("settings.txt", "r")) != NULL){
    fscanf(fr, "%i %i %i ", &minemax, &size_x, &size_y);

    SCREEN_HEIGHT = 50 + size_y*38 + 20;
    SCREEN_WIDTH = 100 + size_x*38;
    if(SCREEN_WIDTH < 770)
      SCREEN_WIDTH = 770;
    if(minemax >= (size_x*size_y - 9))
      minemax = size_x*size_y - 9;
    fclose(fr);
  }
  if(SDL_Init( SDL_INIT_EVERYTHING ) == -1)
    return 1;
  if((screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE)) == NULL)
    return 1;

  SDL_WM_SetCaption( "Minesweeper by Mozes 2.0", NULL );
  return 0;
}

int load(){
  if((background = load_image( "background.png" )) == NULL)
    return 1;
   if((scoreboard = load_image( "scoreboard.png" )) == NULL)
    return 1;
  if((buttonClip = load_image( "skin.png" ))== NULL)
    return 1;
  if((nuke = load_image( "boom.png" ))== NULL)
    return 1;
  if((success = load_image( "success.png" ))== NULL)
    return 1;
  if((counter = load_image("minecounter.png")) == NULL)
    return 1;
  if((numberClip = load_image("font.png")) == NULL)
    return 1;
  if((clockimg = load_image("clock.png")) == NULL)
    return 1;
  return 0;
}

void clean_up(){
  SDL_FreeSurface( background );
  SDL_FreeSurface( buttonClip );
  SDL_FreeSurface( numberClip );
  SDL_FreeSurface( nuke );
  SDL_FreeSurface( success );
  SDL_FreeSurface( counter );
  SDL_FreeSurface( screen );
  SDL_FreeSurface( clockimg );
  SDL_Quit();
  return;
}

button *gettile(button **field){
  int x = (event.motion.x-(SCREEN_WIDTH-size_x*38)/2)/38;
  int y = (event.motion.y-50)/38;
  if(x < 0 || y < 0 || x >= size_x || y >= size_y)
    return last_active;
  return(&field[x][y]);
}

Timer gametime;   //timer... ANOTHER HIDDEN THING :D

void generate_field(button **field){
  int x = (event.motion.x-(SCREEN_WIDTH-size_x*38)/2)/38;
  int y = (event.motion.y-50)/38;

  if(x < 0 || y < 0 || x >= size_x || y >= size_y)
    return;
  gametime.start();
  if(x > 0 && y > 0)                field[x-1][y-1].setgen();
  if(y > 0)                         field[x][y-1].setgen();
  if(x < size_x-1 && y > 0)         field[x+1][y-1].setgen();

  if(x > 0)                         field[x-1][y].setgen();
  if(true)                          field[x][y].setgen();
  if(x < size_x-1)                  field[x+1][y].setgen();

  if(x > 0 && y < size_y-1)         field[x-1][y+1].setgen();
  if(y < size_y-1)                  field[x][y+1].setgen();
  if(x < size_x-1 && y < size_y-1)  field[x+1][y+1].setgen();
  srand (time(NULL));
  int mines = 0;
  int stack_limit = 100000; //
  while( mines < minemax){
    for(int i = 0; i < size_x; i++){
      for(int j = 0; j < size_y; j++){
        if(((rand() % 250) == 0) && (field[i][j].mineflag == false) && field[i][j].isavaileable()){
          if(mines == minemax)
            break;
          field[i][j].plantmine();   //every tile has 10% chance to spawn a mine
          mines++;
        }
      }
    }
    if(!stack_limit--)
      break;
  }
  for(int i = 0; i < size_x; i++){
    for(int j = 0; j < size_y; j++){
      if(field[i][j].ismine()){
          if(i > 0 && j > 0)                field[i-1][j-1].addmine();
          if(j > 0)                         field[i][j-1].addmine();
          if(i < size_x-1 && j > 0)         field[i+1][j-1].addmine();

          if(i > 0)                         field[i-1][j].addmine();
          if(true)                          field[i][j].addmine();
          if(i < size_x-1)                  field[i+1][j].addmine();

          if(i > 0 && j < size_y-1)         field[i-1][j+1].addmine();
          if(j < size_y-1)                  field[i][j+1].addmine();
          if(i < size_x-1 && j < size_y-1)  field[i+1][j+1].addmine();
      }
    }
  }
  generated = true;
}

void oracle(button **field, int x, int y){ // if you click a field with a 0, this reveals all the things nearby
  if(x < 0 || y < 0 || x >= size_x || y >= size_y)
    return;
  if(field[x][y].isflag())
    return;
  if(field[x][y].isrevealed())
    return;
  if(field[x][y].getminecount()){
    field[x][y].reveal();
    return;
  }
  if(field[x][y].getminecount() == 0){
    if(field[x][y].ismine()){return;}
    field[x][y].reveal();
    if(x > 0 && y > 0)                oracle(field, x-1, y -1);
    if(y > 0)                         oracle(field, x, y -1);
    if(x < size_x-1 && y > 0)         oracle(field, x+1, y -1);

    if(x > 0)                         oracle(field, x -1, y);
    if(x < size_x-1)                  oracle(field, x +1, y);

    if(x > 0 && y < size_y-1)         oracle(field, x-1, y+1);
    if(y < size_y-1)                  oracle(field, x, y+1);
    if(x < size_x-1 && y < size_y-1)  oracle(field, x+1, y+1);
  }
  return;
}
//if you lose

void makenumber(int number, int off_x, int off_y){
  if(number < 0)
    return;

  int backup = number;
  int numberlen = 1;
  while(number / 10){
    number /= 10;
    numberlen++;
  }
  number = backup;
  for(int i = 0; i < 3; i++)
      apply_surface(off_x + (3-i-1)*50, off_y, background, screen, &clips[number%10+15]);

  for(int i = 0; i < numberlen; i++){
    apply_surface(off_x + (numberlen-i-1)*50, off_y, numberClip, screen, &clips[number%10+15]);
    number -= number %10;
    number /= 10;
  }
  return;
}

void maketime(int number, int off_x, int off_y){
  makenumber(number/60, off_x, off_y);
  int backup = number;
  number /= 60;
  int numberlen = 1;
  while(number / 10){
    number /= 10;
    numberlen++;
  }
  number = backup;
  apply_surface(off_x + (numberlen)*50, off_y, numberClip, screen, &clips[25]);
  makenumber(number%60, (numberlen+1)*50 + off_x, off_y);
}

void boom(button **field){
  for(int i = 0; i < size_x; i++){
    for(int j =0; j < size_y; j++){
      if(field[i][j].ismine()){
        field[i][j].reveal();
      }
      if(field[i][j].isflag() && !field[i][j].ismine()){
        field[i][j].changestate(M_FALSE);

        field[i][j].reveal();
      }
    }
  }
  gametime.pause();

  SDL_Flip( screen);
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

  apply_surface(SCREEN_WIDTH/2-320, SCREEN_HEIGHT/2 - 240, nuke, screen);
    FILE *fr;
  int m, x, y, t, timetotal = 0, tryes = 1, besttime = 2147483647, fails = 1;

  if((fr = fopen("records", "r"))!= NULL){
    while(fscanf(fr, "%i %i %i %i", &m, &x, &y, &t) == 4){
      if(m == minemax && x == size_x && y == size_y){
        if(t > 0)
          besttime = besttime < t ? besttime : t;
        tryes++;
        fails += t ? 0: 1;
      }
    }
  }
  apply_surface(SCREEN_WIDTH/2-250, SCREEN_HEIGHT/2-100, scoreboard, screen);

  makenumber(((tryes - fails)*100)/tryes, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 -100);
  SDL_Rect clip = {250, 0, 50, 50};
  apply_surface(SCREEN_WIDTH/2+100, SCREEN_HEIGHT/2 -100, numberClip, screen, &clip);
  maketime(besttime == 2147483647 ? 0: besttime, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 -50);
  SDL_Flip( screen);
  fclose(fr);


  stopflag = true;
  while(stopflag){
    if(SDL_PollEvent( &event )){
      if(event.type == SDL_QUIT)
        return;
      if(event.type == SDL_MOUSEBUTTONUP)
        stopflag = false;
    }
    SDL_Delay(50);
  }
}

//wheel click... reveals all nearby tiles
void reveal_near(button **field){
  int x = (event.motion.x-(SCREEN_WIDTH-size_x*38)/2)/38;
  int y = (event.motion.y-50)/38;

  if(x < 0 || y < 0 || x >= size_x || y >= size_y)
    return;

  int adj_flags = 0;
  if(x > 0 && y > 0 && field[x-1][y-1].isflag())                  adj_flags++;
  if(y > 0 && field[x][y-1].isflag())                             adj_flags++;
  if(x < size_x-1 && y > 0 && field[x+1][y-1].isflag())           adj_flags++;

  if(x > 0 && field[x-1][y].isflag())                             adj_flags++;
  if(x < size_x-1 && field[x+1][y].isflag())                      adj_flags++;

  if(x > 0 && y < size_y-1 && field[x-1][y+1].isflag())           adj_flags++;
  if(y < size_y-1 && field[x][y+1].isflag())                      adj_flags++;
  if(x < size_x-1 && y < size_y-1 && field[x+1][y+1].isflag())    adj_flags++;

  if(adj_flags != field[x][y].getminecount())
    return;

  if(!field[x][y].isrevealed())
    return;
  if(x > 0 && y > 0){
    if(field[x-1][y-1].getminecount() == 0){
      oracle(field, x-1, y-1);}
    else{ field[x-1][y-1].reveal();}
  }
  if(y > 0){
    if(field[x][y-1].getminecount() == 0){
      oracle(field, x, y-1);}
    else{field[x][y-1].reveal();}
  }

  if(x < size_x-1 && y > 0) {
    if(field[x+1][y-1].getminecount() == 0){
      oracle(field, x+1, y-1);}
    else{field[x+1][y-1].reveal();}
  }

  if(x > 0) {
    if(field[x-1][y].getminecount() == 0){
      oracle(field, x-1, y);}
    else{field[x-1][y].reveal();}
  }
  if(x < size_x-1){
    if(field[x+1][y].getminecount() == 0){
      oracle(field, x+1, y);}
    else{field[x+1][y].reveal();}
  }

  if(x > 0 && y < size_y-1){
    if(field[x-1][y+1].getminecount() == 0){
      oracle(field, x-1, y+1);}
    else{field[x-1][y+1].reveal();}
  }
  if(y < size_y-1){
    if(field[x][y+1].getminecount() == 0){
      oracle(field, x, y+1);}
    else{field[x][y+1].reveal();}
  }
  if(x < size_x-1 && y < size_y-1){
    if(field[x+1][y+1].getminecount() == 0){
      oracle(field, x+1, y+1);}
    else{field[x+1][y+1].reveal();}
  }
}

//this is for the flagcounter...

void count_flags(button **field){
  flagcounter = minemax;
  for(int i = 0; i < size_x; i++){
    for(int j = 0; j < size_y; j++){
      if(field[i][j].isflag())
        flagcounter--;
    }
  }
  return;
}

bool checkwin(button **field){
  fprintf(out, "Checking board for victory...\n");
  bool winflag = true;
  for(int i = 0; i < size_x; i++){
    for(int j = 0; j < size_y; j++){
      if(!field[i][j].isrevealed() && !field[i][j].isflag()){
        fprintf(out, "Tile[%i][%i] Is Wrong...\n", i, j);
        winflag = false;
      }
      if(field[i][j].isflag() && !field[i][j].ismine()){
        fprintf(out, "Tile[%i][%i] Is Wrong...\n", i, j);
        winflag = false;
      }
    }
  }
  fprintf(out, "All is fine");
  return winflag;
}
// sth like boom... but not as fun :D
void victory(){
  apply_surface(SCREEN_WIDTH/2-320, SCREEN_HEIGHT/2 - 96, success, screen);

  bool stopflag = true;
  gametime.pause();
  FILE *fr;
  int m, x, y, t, timetotal = 0, tryes = 1, besttime = gametime.get_ticks()/1000, fails = 0;

  if((fr = fopen("records", "r"))!= NULL){
    while(fscanf(fr, "%i %i %i %i", &m, &x, &y, &t) == 4){
      if(m == minemax && x == size_x && y == size_y){
        if(t > 0)
          besttime = besttime < t ? besttime : t;
        tryes++;
        fails += t ? 0: 1;
      }
    }
  }

  apply_surface(SCREEN_WIDTH/2-250, SCREEN_HEIGHT/2 +96, scoreboard, screen);

  makenumber(((tryes - fails)*100)/tryes, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 +96);
  SDL_Rect clip = {250, 0, 50, 50};
  if(((tryes - fails)*100)/tryes < 100)
    apply_surface(SCREEN_WIDTH/2+100, SCREEN_HEIGHT/2 +96, numberClip, screen, &clip);
  else{apply_surface(SCREEN_WIDTH/2+150, SCREEN_HEIGHT/2 +96, numberClip, screen, &clip);}

  maketime(besttime, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 +146);
  SDL_Flip( screen);
  fclose(fr);



  while(stopflag){
    if(SDL_PollEvent( &event )){
      if(event.type == SDL_QUIT)
        return;
      if(event.type == SDL_MOUSEBUTTONUP)
        stopflag = false;
    }
    SDL_Delay(50);
  }
}

// prints a number on the screen using the clip... and stuff...


void handle_events(button **field){
  button *active;
  if( event.type == SDL_MOUSEMOTION ){
    active = gettile(field);

    if(last_active != NULL && last_active != active){
      last_active->changestate(M_OUT);
    }
    if(event.button.button == SDL_BUTTON_LEFT){
      if(active != NULL)
        active->changestate(M_DOWN);
    }
    else{
      if(active != NULL)
        active->changestate(M_HOVER);
    }
    if(active != NULL)
    last_active = active;
  }

  if( event.type == SDL_MOUSEBUTTONDOWN ){
    if( event.button.button == SDL_BUTTON_LEFT ){
      active = gettile(field);
      active->changestate(M_DOWN);
    }
  }
  if( event.type == SDL_MOUSEBUTTONUP ){
    if( event.button.button == SDL_BUTTON_LEFT ){
      if(!generated){
        generate_field(field);
      }
      active = gettile(field);
      if(active->isrevealed()){
        int i = 0;
        while(i < 25){
          if(SDL_PollEvent( &event )){
            if(event.type == SDL_MOUSEBUTTONUP){
              if(event.button.button == SDL_BUTTON_LEFT){
                reveal_near(field);
                break;
              }
            }
          }
          i++;
          SDL_Delay(20);
        }
      }
      if(active == gettile(field))
        oracle(field, (event.motion.x-(SCREEN_WIDTH-size_x*38)/2)/38, (event.motion.y-50)/38);
      if(!flagcounter){
        if(checkwin(field)){gameover = 2;}
      }
    }
    if( event.button.button == SDL_BUTTON_RIGHT ){
      active = gettile(field);
      active->changeflag();
      count_flags(field);
      if(!flagcounter){
        if(checkwin(field)){gameover = 2;}
      }
    }
    if( event.button.button == SDL_BUTTON_MIDDLE ){
      active = gettile(field);
      reveal_near(field);
      if(!flagcounter){
        if(checkwin(field)){gameover = 2;}
      }
    }
  }
}

int main(int argc, char *argv[]){
  if(init() || load())
    return 1;
  Timer fps;
  apply_surface(0, 0, background, screen);
  apply_surface( 50, 0, counter, screen );
  apply_surface( SCREEN_WIDTH/3*2, 0, clockimg, screen );

  button **field = new button*[size_x];
  for(int i = 0; i < size_x; i++)
    field[i] = new button[size_y];

  for(int i = 0; i < size_x; i++)
    for(int j = 0; j < size_y; j++)
      field[i][j].setbutton( (SCREEN_WIDTH-size_x*38)/2 + i*38, 50 + j*38, 38, 38);

  for(int i = 0; i < size_x; i++)
    for(int j = 0; j < size_y; j++)
      field[i][j].show();

  bool quitflag = false;
  int flagcountercmp = minemax;
  flagcounter = minemax;
  makenumber(flagcounter, SCREEN_WIDTH/20+320, 0);
  maketime(gametime.get_ticks()/1000 , SCREEN_WIDTH/3*2+50, 0);

  while(quitflag == false){
    fps.start();
    maketime(gametime.get_ticks()/1000 , SCREEN_WIDTH/3*2+50, 0);
    if(gameover == 1){
      boom(field);
      break;
    }
    if(gameover == 2){
      victory();
      break;
    }
    if(SDL_PollEvent( &event )){
      if(event.type == SDL_QUIT){
        quitflag = true;
        gameover = 1;
      }
      handle_events(field);
    }
    if(flagcountercmp != flagcounter){
      flagcountercmp = flagcounter;
      makenumber(flagcounter, SCREEN_WIDTH/20 + 320, 0);
    }
    SDL_Flip(screen);
    if(( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )){
      SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
    }
  }

  FILE *fa = fopen("records", "a");
  fprintf(fa, "%i %i %i %i \n", minemax, size_x, size_y, (gameover == 2) ? gametime.get_ticks()/1000 : 0);
  fclose(fa);

  gametime.stop();
  for(int i = 0; i < size_x; i++)
    delete [] field[i];
  delete [] field;
  clean_up();
  return 0;
}
