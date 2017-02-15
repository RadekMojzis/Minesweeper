
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>


//Global variables
SDL_Window* window = NULL;
SDL_Renderer* main_renderer = NULL;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

SDL_Texture* button_textures = NULL;

bool skip_menu_flag = false;
const int M_HOVER = 0;
const int M_OUT = 1;
const int M_DOWN = 2;
SDL_Event event;
//Classes

class button{
  private:
    SDL_Rect position;
		int displace_x;
		int displace_y;
		
    SDL_Rect original_clip;
    SDL_Rect current_clip;
    
    int state = M_OUT;
    int (*f_ptr)();
    
  public:
	  void handle_mouse();
    void adjust_position();
    void render();
    void changestate(int newstate);
    button(SDL_Rect position, SDL_Rect clip, int (*foo)() );
};

void button::handle_mouse(){
	switch (event.type)  {
    case SDL_MOUSEBUTTONDOWN: {
			if(event.motion.x >= position.x && event.motion.x <= position.x + position.w
			&& event.motion.y >= position.y && event.motion.y <= position.y + position.h)
			  changestate(M_DOWN);
			else{changestate(M_OUT);}
			break;
		}
    case SDL_MOUSEMOTION: {
			if(event.motion.x >= position.x && event.motion.x <= position.x + position.w
			&& event.motion.y >= position.y && event.motion.y <= position.y + position.h)
			  changestate(M_HOVER);
			else{changestate(M_OUT);}
			break;
		}
    case SDL_MOUSEBUTTONUP: {
			if(event.motion.x >= position.x && event.motion.x <= position.x + position.w
			&& event.motion.y >= position.y && event.motion.y <= position.y + position.h)
				f_ptr();
			else{changestate(M_OUT);}
			break;
		}

  }
}

button* existing_buttons[10000];
int buttoncount = 0;

void button::render(){
  SDL_RenderCopy(main_renderer, button_textures, &current_clip, &position);
}

void button::changestate(int newstate){
  if(newstate == M_HOVER){
    current_clip.x = original_clip.x + 300;
    render();
  }
  if(newstate == M_DOWN){  
    current_clip.x = original_clip.x +600;
    render();
  }
  if(newstate == M_OUT){
    current_clip.x = original_clip.x;
    render();
  }
}

button::button(SDL_Rect poss, SDL_Rect clip, int (*foo)() ){
  existing_buttons[buttoncount++] = this;
  position = poss;
  displace_x = position.x;
  displace_y = position.y;  
  adjust_position();
  
  original_clip = clip;
  current_clip = clip;
  state = M_OUT;
  f_ptr = foo;
  render();
}

void button::adjust_position(){
  position.x = SCREEN_WIDTH/2-position.w/2 + displace_x;
  position.y = SCREEN_HEIGHT/2-position.h/2 + displace_y;
	render();
}

//Function declarations
SDL_Texture* loadTexture( std::string path );
bool init();
void deinit();
bool load_menu();
void display_menu();
bool load_game();
void menu_handle_events();
int new_game();
int options();
int records();
int quit();

int main(int argc, char* args[]){
	if(init()) return 1;
	if(!skip_menu_flag) 
	  if(load_menu()) return 1;
	else
		if(load_game()) return 1;
	
	return 0;
}

bool init(){
	window = SDL_CreateWindow( "Minesweeper", 
														SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
														SCREEN_WIDTH, SCREEN_HEIGHT, 
														SDL_WINDOW_RESIZABLE );
														
  main_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(window == NULL || main_renderer == NULL) return true;
	return false;
}

void deinit(){
	SDL_DestroyRenderer(main_renderer);
	//SDL_DestroyTexture(button_texture);
  SDL_DestroyWindow( window );
	SDL_Quit();
}

bool load_menu(){
	button_textures  = loadTexture("menu.bmp");
	display_menu();
	return false;
}

void display_menu(){
	SDL_SetRenderDrawColor( main_renderer, 0xF0, 0xF0, 0xF0, 0xFF );
	SDL_RenderClear( main_renderer );
  int spacing = 75;
  button NEW_GAME_BTN({0, 2*spacing, 150, 50},{0,0,300,100}, &new_game );
  button RECORDS_BTN({0, spacing, 150, 50},{0,100,300,100}, &records );
  button OPTIONS_BTN({0, 0, 150, 50},{0,200,300,100}, &options );
  button QUIT_BTN({0, -spacing, 150, 50},{0,300,300,100}, &quit );  
  
  bool die = false;
  while( !die ){
	  while( SDL_PollEvent( &event ) != 0 ){
			if( event.type == SDL_QUIT ){
				die = true;
			}
      menu_handle_events();
	  }
		SDL_RenderPresent( main_renderer );
		SDL_Delay(20);
	}
  
	return;
}

bool load_game(){
	return false;
}

SDL_Texture* loadTexture( std::string path ){
  return SDL_CreateTextureFromSurface( main_renderer, IMG_Load( path.c_str() ));
}

int new_game(){
  return 0;
}
int options(){
  return 0;
}
int records(){
  return 0;
}
int quit(){
  return 0;
}

void menu_handle_events(){
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
        existing_buttons[i]->handle_mouse();
		  }
		}
    case SDL_MOUSEMOTION: {
			for(int i = buttoncount-1; i >= 0; i--){
        existing_buttons[i]->handle_mouse();
		  }
			break;
		}
    case SDL_MOUSEBUTTONUP: {
			for(int i = buttoncount-1; i >= 0; i--){
        existing_buttons[i]->handle_mouse();
		  }
			break;
		}
  }
}

