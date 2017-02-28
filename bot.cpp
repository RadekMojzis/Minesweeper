
#include "bot.h"
#include "minefield.h"
#include <stdio.h>
extern SDL_Renderer* main_renderer;
int byuser = 1;
bool is_solvable(minefield* field, int start_x, int start_y);
extern FILE *out;
int changeflag = 0;
void use_bot(minefield* field){
	changeflag = 0;
  for(int i = 0; i < field->tiles_x; i++)
    for(int j = 0; j < field->tiles_y; j++){
      if(field->tiles[i][j].revealedflag){
				if(field->tiles[i][j].mines_around){
					if(n_o_adjucent_flags(field, i, j) == field->tiles[i][j].mines_around){  
						if(n_o_unrevealed_tiles(field, i, j) != 0){
							byuser = 0;
							field->reveal_near(i, j);
							byuser = 1;
							changeflag = 1;
						}
					}
					else if(field->tiles[i][j].mines_around == (n_o_unrevealed_tiles(field, i, j) + n_o_adjucent_flags(field, i, j))){
						put_flags_around(field, i, j);
						//fprintf(out, "Log - [%i][%i] - setting flags\n", i+1, j+1);
						changeflag = 1;
					}	
        }
			}
    }

  if(!changeflag){
		for(int i = 0; i < field->tiles_x; i++){
      for(int j = 0; j < field->tiles_y; j++){
		    set_danger(field, i, j);
			}
		}
	}
  return;
}

int put_flags_around(minefield * field, int i, int j){

  if(i > 0 && j > 0 && !field->tiles[i-1][j-1].flagflag && !field->tiles[i-1][j-1].danger)                                field->tiles[i-1][j-1].toggle_flag();
  if(j > 0 && !field->tiles[i][j-1].flagflag && !field->tiles[i][j-1].danger)                                             field->tiles[i][j-1].toggle_flag();
  if(i < field->tiles_x-1 && j > 0 && !field->tiles[i+1][j-1].flagflag && !field->tiles[i+1][j-1].danger)                 field->tiles[i+1][j-1].toggle_flag();

  if(i > 0 && !field->tiles[i-1][j].flagflag && !field->tiles[i-1][j].danger)                                             field->tiles[i-1][j].toggle_flag();
  if(i < field->tiles_x-1 && !field->tiles[i+1][j].flagflag && !field->tiles[i+1][j].danger)                              field->tiles[i+1][j].toggle_flag();

  if(i > 0 && j < field->tiles_y-1 && !field->tiles[i-1][j+1].flagflag && !field->tiles[i-1][j+1].danger)                 field->tiles[i-1][j+1].toggle_flag();
  if(j < field->tiles_y-1 && !field->tiles[i][j+1].flagflag&& !field->tiles[i][j+1].danger)                               field->tiles[i][j+1].toggle_flag();
  if(i < field->tiles_x-1 && j < field->tiles_y-1 && !field->tiles[i+1][j+1].flagflag && !field->tiles[i+1][j+1].danger)  field->tiles[i+1][j+1].toggle_flag();
}

int n_o_unrevealed_tiles(minefield * field, int i, int j){
  int unrevealed_tiles = 0;
  if(i > 0 && j > 0 && (!field->tiles[i-1][j-1].revealedflag && !field->tiles[i-1][j-1].flagflag && !field->tiles[i-1][j-1].danger))                                unrevealed_tiles++;
  if(j > 0 && (!field->tiles[i][j-1].revealedflag && !field->tiles[i][j-1].flagflag && !field->tiles[i][j-1].danger))                                               unrevealed_tiles++;
  if(i < field->tiles_x-1 && j > 0 && (!field->tiles[i+1][j-1].revealedflag && !field->tiles[i+1][j-1].flagflag && !field->tiles[i+1][j-1].danger))                 unrevealed_tiles++;

  if(i > 0 && (!field->tiles[i-1][j].revealedflag && !field->tiles[i-1][j].flagflag && !field->tiles[i-1][j].danger))                                               unrevealed_tiles++;
  if(i < field->tiles_x-1 && (!field->tiles[i+1][j].revealedflag && !field->tiles[i+1][j].flagflag && !field->tiles[i+1][j].danger) )                               unrevealed_tiles++;

  if(i > 0 && j < field->tiles_y-1 && (!field->tiles[i-1][j+1].revealedflag && !field->tiles[i-1][j+1].flagflag && !field->tiles[i-1][j+1].danger))                 unrevealed_tiles++;
  if(j < field->tiles_y-1 && (!field->tiles[i][j+1].revealedflag && !field->tiles[i][j+1].flagflag && !field->tiles[i][j+1].danger))                                unrevealed_tiles++;
  if(i < field->tiles_x-1 && j < field->tiles_y-1 && (!field->tiles[i+1][j+1].revealedflag && !field->tiles[i+1][j+1].flagflag && !field->tiles[i+1][j+1].danger))  unrevealed_tiles++;
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


void set_danger(minefield *field, int i, int j){
	if(!field->tiles[i][j].revealedflag || !field->tiles[i][j].mines_around)
		return;
	if(n_o_unrevealed_tiles(field, i, j) == 0) return;
		
	int unrevealed_mines = (field->tiles[i][j].mines_around - n_o_adjucent_flags(field, i, j));
	int out_of = n_o_unrevealed_tiles(field, i, j);
	
	if(i > 0 && j > 0 && (!field->tiles[i-1][j-1].revealedflag && !field->tiles[i-1][j-1].flagflag))                                field->tiles[i-1][j-1].danger = 1;
  if(j > 0 && (!field->tiles[i][j-1].revealedflag && !field->tiles[i][j-1].flagflag))                                             field->tiles[i][j-1].danger = 1;
  if(i < field->tiles_x-1 && j > 0 && (!field->tiles[i+1][j-1].revealedflag && !field->tiles[i+1][j-1].flagflag))                 field->tiles[i+1][j-1].danger = 1;

  if(i > 0 && (!field->tiles[i-1][j].revealedflag && !field->tiles[i-1][j].flagflag))                                             field->tiles[i-1][j].danger = 1;
  if(i < field->tiles_x-1 && (!field->tiles[i+1][j].revealedflag && !field->tiles[i+1][j].flagflag) )                             field->tiles[i+1][j].danger = 1;

  if(i > 0 && j < field->tiles_y-1 && (!field->tiles[i-1][j+1].revealedflag && !field->tiles[i-1][j+1].flagflag))                 field->tiles[i-1][j+1].danger = 1;
  if(j < field->tiles_y-1 && (!field->tiles[i][j+1].revealedflag && !field->tiles[i][j+1].flagflag))                              field->tiles[i][j+1].danger = 1;
  if(i < field->tiles_x-1 && j < field->tiles_y-1 && (!field->tiles[i+1][j+1].revealedflag && !field->tiles[i+1][j+1].flagflag))  field->tiles[i+1][j+1].danger = 1;
	
	//fprintf(out, "setting danger around [%i][%i], mines - %i, unrevealed mines - %i, unrevealed_tiles - %i\n", i+1, j+1,field->tiles[i][j].mines_around, unrevealed_mines, out_of);
	//fflush(out);
	eval_danger(field, unrevealed_mines, out_of);
	
	if(i > 0 && j > 0)                                field->tiles[i-1][j-1].danger = 0;
  if(j > 0)                                             field->tiles[i][j-1].danger = 0;
  if(i < field->tiles_x-1 && j > 0)                 field->tiles[i+1][j-1].danger = 0;

  if(i > 0)                                             field->tiles[i-1][j].danger = 0;
  if(i < field->tiles_x-1)                             field->tiles[i+1][j].danger = 0;

  if(i > 0 && j < field->tiles_y-1)                 field->tiles[i-1][j+1].danger = 0;
  if(j < field->tiles_y-1)                              field->tiles[i][j+1].danger = 0;
  if(i < field->tiles_x-1 && j < field->tiles_y-1)  field->tiles[i+1][j+1].danger = 0;
			
	
	return;
}

int crazy_haluznumero(int of, int mines, int danger){
	if(of == 2 && mines == 1 && danger == 2) return 1;
	if(of == 3 && mines == 1 && danger == 3) return 1;
	if(of == 3 && mines == 2 && danger == 3) return 2;
	return 0;
}

void eval_danger(minefield *field, int mines, int out_of){
	for(int i = 0; i < field->tiles_x; i++){
		for(int j = 0; j < field->tiles_y; j++){
		  if(field->tiles[i][j].revealedflag && field->tiles[i][j].mines_around){
				int adj_danger = 0;
				if(i > 0 && j > 0)                                    adj_danger += field->tiles[i-1][j-1].danger;
				if(j > 0)                                             adj_danger += field->tiles[i][j-1].danger;
				if(i < field->tiles_x-1 && j > 0)                     adj_danger += field->tiles[i+1][j-1].danger;

				if(i > 0)                                             adj_danger += field->tiles[i-1][j].danger;
				if(i < field->tiles_x-1)                              adj_danger += field->tiles[i+1][j].danger;
				
				if(i > 0 && j < field->tiles_y-1)                     adj_danger += field->tiles[i-1][j+1].danger;
				if(j < field->tiles_y-1)                              adj_danger += field->tiles[i][j+1].danger;
				if(i < field->tiles_x-1 && j < field->tiles_y-1)      adj_danger += field->tiles[i+1][j+1].danger;
				
				if(adj_danger){
					if(field->tiles[i][j].mines_around - n_o_adjucent_flags(field, i, j)- (mines - ( out_of - adj_danger)) == 0){
						byuser = 0;
						fprintf(out, "reveal!\n");
						fflush(out);
						field->reveal_near(i, j);
						fprintf(out, "done...\n");
						fflush(out);
						byuser = 1;
						//fprintf(out, "revealing around [%i][%i]", i, j);
					}
					else{
						if(crazy_haluznumero(out_of, mines, adj_danger))
						if(n_o_adjucent_flags(field, i, j) + crazy_haluznumero(out_of, mines, adj_danger) + n_o_unrevealed_tiles(field, i, j) == field->tiles[i][j].mines_around){
							put_flags_around(field, i, j);
							SDL_RenderPresent( main_renderer );
							//SDL_Delay(5000);
					  }
				  }
			  }
		  }
	  }
	}
}

