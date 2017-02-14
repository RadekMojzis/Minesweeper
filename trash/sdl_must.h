#ifndef SDL_MUST_INCLUDE
#define SDL_MUST_INCLUDE

#include "timer.h"

void apply_surface( int x, int y, SDL_Surface* from, SDL_Surface* to, SDL_Rect* clip = NULL );

SDL_Surface *load_image( std::string filename );

#endif 