
#include "bot.h"
#include "minefield.h"

bool is_solvable(minefield* field, int start_x, int start_y);
extern FILE* out;
void use_bot(minefield* field){
  for(int i = 0; i < field->tiles_x; i++)
    for(int j = 0; j < field->tiles_y; j++){
      if(field->tiles[i][j].revealedflag){
        if(field->tiles[i][j].mines_around){
          if(n_o_adjucent_flags(field, i, j) == field->tiles[i][j].mines_around){
            field->reveal_near(i, j);
          }
          else{
            fprintf(out, "LOG - tile [%i][%i] - Mines %i - Unrevealed %i - Flags %i - \n", i+1, j+1, field->tiles[i][j].mines_around, n_o_unrevealed_tiles(field, i, j), n_o_adjucent_flags(field, i, j));
            if(field->tiles[i][j].mines_around == (n_o_unrevealed_tiles(field, i, j) + n_o_adjucent_flags(field, i, j))){
              put_flags_around(field, i, j);
            }
          }
        }
      }
    }
  
  return;
}

int put_flags_around(minefield * field, int i, int j){
  fprintf(out, "puting flags around [%i][%i]\n", i, j);
  if(i > 0 && j > 0 && !field->tiles[i-1][j-1].flagflag)                                field->tiles[i-1][j-1].toggle_flag();
  if(j > 0 && !field->tiles[i][j-1].flagflag)                                           field->tiles[i][j-1].toggle_flag();
  if(i < field->tiles_x-1 && j > 0 && !field->tiles[i+1][j-1].flagflag)                 field->tiles[i+1][j-1].toggle_flag();

  if(i > 0 && !field->tiles[i-1][j].flagflag)                                           field->tiles[i-1][j].toggle_flag();
  if(i < field->tiles_x-1 && !field->tiles[i+1][j].flagflag)                            field->tiles[i+1][j].toggle_flag();

  if(i > 0 && j < field->tiles_y-1 && !field->tiles[i-1][j+1].flagflag)                 field->tiles[i-1][j+1].toggle_flag();
  if(j < field->tiles_y-1 && !field->tiles[i][j+1].flagflag)                            field->tiles[i][j+1].toggle_flag();
  if(i < field->tiles_x-1 && j < field->tiles_y-1 && !field->tiles[i+1][j+1].flagflag)  field->tiles[i+1][j+1].toggle_flag();
}

int n_o_unrevealed_tiles(minefield * field, int i, int j){
  int unrevealed_tiles = 0;
  if(i > 0 && j > 0 && (!field->tiles[i-1][j-1].revealedflag && !field->tiles[i-1][j-1].flagflag))                                unrevealed_tiles++;
  if(j > 0 && (!field->tiles[i][j-1].revealedflag && !field->tiles[i][j-1].flagflag))                                             unrevealed_tiles++;
  if(i < field->tiles_x-1 && j > 0 && (!field->tiles[i+1][j-1].revealedflag && !field->tiles[i+1][j-1].flagflag))                 unrevealed_tiles++;

  if(i > 0 && (!field->tiles[i-1][j].revealedflag && !field->tiles[i-1][j].flagflag))                                             unrevealed_tiles++;
  if(i < field->tiles_x-1 && (!field->tiles[i+1][j].revealedflag && !field->tiles[i+1][j].flagflag) )                             unrevealed_tiles++;

  if(i > 0 && j < field->tiles_y-1 && (!field->tiles[i-1][j+1].revealedflag && !field->tiles[i-1][j+1].flagflag))                 unrevealed_tiles++;
  if(j < field->tiles_y-1 && (!field->tiles[i][j+1].revealedflag && !field->tiles[i][j+1].flagflag))                              unrevealed_tiles++;
  if(i < field->tiles_x-1 && j < field->tiles_y-1 && (!field->tiles[i+1][j+1].revealedflag && !field->tiles[i+1][j+1].flagflag))  unrevealed_tiles++;
  return unrevealed_tiles;
}

int n_o_adjucent_flags(minefield * field, int i, int j){
  int adj_flags = 0;
  if(i > 0 && j > 0 && field->tiles[i-1][j-1].flagflag)                                adj_flags++;
  if(j > 0 && field->tiles[i][j-1].flagflag)                                           adj_flags++;
  if(i < field->tiles_x-1 && j > 0 && field->tiles[i+1][j-1].flagflag)                 adj_flags++;

  if(i > 0 && field->tiles[i-1][j].flagflag)                                           adj_flags++;
  if(i < field->tiles_x-1 && field->tiles[i+1][j].flagflag)                            adj_flags++;

  if(i > 0 && j < field->tiles_y-1 && field->tiles[i-1][j+1].flagflag)                 adj_flags++;
  if(j < field->tiles_y-1 && field->tiles[i][j+1].flagflag)                            adj_flags++;
  if(i < field->tiles_x-1 && j < field->tiles_y-1 && field->tiles[i+1][j+1].flagflag)  adj_flags++;
  return adj_flags;
}