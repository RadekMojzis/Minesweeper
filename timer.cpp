
#include "timer.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdio.h>


Timer::Timer(){
  start_ticks = 0;
  paused_ticks = 0;
  running = false;
}


void Timer::start(){
  running = true;
  start_ticks = SDL_GetTicks() - paused_ticks;
  paused_ticks = 0;
}

void Timer::stop(){
  running = false;
  paused_ticks = 0;
  start_ticks = 0;
}

void Timer::pause(){
  paused_ticks = SDL_GetTicks();
  running = false;
}

int Timer::get_ticks(){
  return running? SDL_GetTicks() - start_ticks : paused_ticks-start_ticks;
}

bool Timer::is_running(){
  return running;
}