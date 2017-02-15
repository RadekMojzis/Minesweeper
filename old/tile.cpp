
#include "tile.h"

void reveal_near(tile **field);

bool tile::isflag(){
  return flag;
}

bool tile::isrevealed(){
  return revealed;
}

int tile::getminecount(){
  return minecount;
}

bool tile::isavaileable(){
  return !gen;
}

void tile::setgen(){
  gen = true;
}

void tile::changeflag(){
  if(revealed)
    return;
  flag = flag ? false : true;
  clip = flag? &clips[12] : &clips[M_HOVER];
  show();
}

void tile::reveal(){
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

tile::tile(){
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

void tile::settile(int x, int y, int w, int h){  //instead of constructor... because... one does not simply inicialise a 2D array of objects with creator..
  box.x = x;  //offset
  box.y = y;
  box.w = w;  //size
  box.h = h;
}

void tile::plantmine(){
  mine = true;
}

bool tile::ismine(){
  return mine;
}

void tile::addmine(){
  minecount++;
}

void tile::show(){
  apply_surface( box.x, box.y, tileClip, screen, clip);
}

void tile::changestate(int newstate){
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
