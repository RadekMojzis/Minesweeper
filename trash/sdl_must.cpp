
#include "sdl_must.h"

void apply_surface( int x, int y, SDL_Surface* from, SDL_Surface* to, SDL_Rect* clip = NULL ){
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( from, clip, to, &offset );
}

SDL_Surface *load_image( std::string filename ){
  SDL_Surface* loadedImage = IMG_Load( filename.c_str() );

  if( loadedImage != NULL ){
    if(loadedImage == NULL)
      return NULL;
    loadedImage = SDL_DisplayFormat( loadedImage );
    if(loadedImage == NULL)
      return NULL;
    SDL_SetColorKey( loadedImage, SDL_SRCCOLORKEY, SDL_MapRGB( loadedImage->format, 0xFF, 0xFF, 0xFF ) );
  }
  return loadedImage;
}

