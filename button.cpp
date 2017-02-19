

#include "button.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

extern SDL_Renderer* main_renderer;
extern SDL_Texture* button_textures;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern FILE* out;

int button::handle_mouse(SDL_Event event){
  
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
			&& event.motion.y >= position.y && event.motion.y <= position.y + position.h){
			  if(event.button.button == SDL_BUTTON_LEFT){
          changestate(M_DOWN);}
        else{
          changestate(M_HOVER);}
      }
			else{changestate(M_OUT);}
			break;
		}
    case SDL_MOUSEBUTTONUP: {
			if(event.motion.x >= position.x && event.motion.x <= position.x + position.w
			&& event.motion.y >= position.y && event.motion.y <= position.y + position.h){
				//identify(func_number);
        return func_number;
      }
			else{changestate(M_OUT);}
			break;
		}

  }
  return 0;
}

void button::render(){
  //fprintf(out, "rendering {%i, %i, %i, %i} {%i, %i, %i, %i}\n", current_clip.x, current_clip.y, current_clip.w, current_clip.h, position.x, position.y, position.w, position.h);
  //if(button_textures == NULL)fprintf(out, "NULL\n");
  //SDL_RenderCopy(main_renderer, button_textures, NULL, NULL);
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

void button::identify(int number){
  
  fprintf(out, "Identifying as %i.\nReturning with %i..\n", func_number, number);
}

button::button(SDL_Rect poss, SDL_Rect clip, int foo){
  position = poss;
  displace_x = position.x;
  displace_y = position.y;  
  adjust_position();
  
  original_clip = clip;
  current_clip = clip;
  state = M_OUT;
  func_number = foo;
  render();
}

void button::adjust_position(){
  position.x = SCREEN_WIDTH/2-position.w/2 + displace_x;
  position.y = SCREEN_HEIGHT/2-position.h/2 + displace_y;
	render();
}