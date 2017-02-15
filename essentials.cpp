

SDL_Texture* loadTexture( std::string path ){
  return SDL_CreateTextureFromSurface( main_renderer, IMG_Load( path.c_str() ));
}