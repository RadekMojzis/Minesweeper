
void set_danger(minefield *field, int i, int j){
	if(!field->tiles[i][j].revealedflag || !field->tiles[i][j].mines_around)
		return;
	if(n_o_unrevealed_tiles(field, i, j) == 0) return;
		
	int danger = (field->tiles[i][j].mines_around - n_o_adjucent_flags(field, i, j)) * 110 / n_o_unrevealed_tiles(field, i, j);

	//fprintf(out, "Log - field [%i][%i] - danger = %i\n", i, j, danger);
	if(i > 0 && j > 0 && (!field->tiles[i-1][j-1].revealedflag && !field->tiles[i-1][j-1].flagflag))                                field->tiles[i-1][j-1].danger = danger;
  if(j > 0 && (!field->tiles[i][j-1].revealedflag && !field->tiles[i][j-1].flagflag))                                             field->tiles[i][j-1].danger = danger;
  if(i < field->tiles_x-1 && j > 0 && (!field->tiles[i+1][j-1].revealedflag && !field->tiles[i+1][j-1].flagflag))                 field->tiles[i+1][j-1].danger = danger;

  if(i > 0 && (!field->tiles[i-1][j].revealedflag && !field->tiles[i-1][j].flagflag))                                             field->tiles[i-1][j].danger = danger;
  if(i < field->tiles_x-1 && (!field->tiles[i+1][j].revealedflag && !field->tiles[i+1][j].flagflag) )                             field->tiles[i+1][j].danger = danger;

  if(i > 0 && j < field->tiles_y-1 && (!field->tiles[i-1][j+1].revealedflag && !field->tiles[i-1][j+1].flagflag))                 field->tiles[i-1][j+1].danger = danger;
  if(j < field->tiles_y-1 && (!field->tiles[i][j+1].revealedflag && !field->tiles[i][j+1].flagflag))                              field->tiles[i][j+1].danger = danger;
  if(i < field->tiles_x-1 && j < field->tiles_y-1 && (!field->tiles[i+1][j+1].revealedflag && !field->tiles[i+1][j+1].flagflag))  field->tiles[i+1][j+1].danger = danger;
	
	
	if(i > 0 && j > 0 && field->tiles[i-1][j-1].revealedflag)                                eval_danger(field, i-1, j-1);
  if(j > 0 && field->tiles[i][j-1].revealedflag)                                           eval_danger(field, i, j-1);
  if(i < field->tiles_x-1 && j > 0 && field->tiles[i+1][j-1].revealedflag)                 eval_danger(field, i+1, j-1);

  if(i > 0 && field->tiles[i-1][j].revealedflag)                                           eval_danger(field, i-1, j);
  if(i < field->tiles_x-1 && field->tiles[i+1][j].revealedflag)                            eval_danger(field, i+1, j);

  if(i > 0 && j < field->tiles_y-1 && field->tiles[i-1][j+1].revealedflag)                 eval_danger(field, i-1, j+1);
  if(j < field->tiles_y-1 && field->tiles[i][j+1].revealedflag)                            eval_danger(field, i, j+1);
  if(i < field->tiles_x-1 && j < field->tiles_y-1 && field->tiles[i+1][j+1].revealedflag)  eval_danger(field, i+1, j+1);
	
	
	if(i > 0 && j > 0)                                    field->tiles[i-1][j-1].danger = 0;
  if(j > 0)                                             field->tiles[i][j-1].danger = 0;
  if(i < field->tiles_x-1 && j > 0)                     field->tiles[i+1][j-1].danger = 0;

  if(i > 0)                                             field->tiles[i-1][j].danger = 0;
  if(i < field->tiles_x-1)                              field->tiles[i+1][j].danger = 0;

  if(i > 0 && j < field->tiles_y-1)                     field->tiles[i-1][j+1].danger = 0;
  if(j < field->tiles_y-1)                              field->tiles[i][j+1].danger = 0;
  if(i < field->tiles_x-1 && j < field->tiles_y-1)      field->tiles[i+1][j+1].danger = 0;
	
	return;
}

void eval_danger(minefield *field, int i, int j){
	if(!field->tiles[i][j].mines_around)
		return;
	
	int adj_danger = 0;
	if(i > 0 && j > 0)                                    adj_danger += field->tiles[i-1][j-1].danger;
  if(j > 0)                                             adj_danger += field->tiles[i][j-1].danger;
  if(i < field->tiles_x-1 && j > 0)                     adj_danger += field->tiles[i+1][j-1].danger;

  if(i > 0)                                             adj_danger += field->tiles[i-1][j].danger;
  if(i < field->tiles_x-1)                              adj_danger += field->tiles[i+1][j].danger;
  
  if(i > 0 && j < field->tiles_y-1)                     adj_danger += field->tiles[i-1][j+1].danger;
  if(j < field->tiles_y-1)                              adj_danger += field->tiles[i][j+1].danger;
  if(i < field->tiles_x-1 && j < field->tiles_y-1)      adj_danger += field->tiles[i+1][j+1].danger;
	
	if(adj_danger < 100)return;
	
	if((field->tiles[i][j].mines_around - n_o_adjucent_flags(field, i, j) - adj_danger/100) == n_o_unrevealed_tiles(field, i, j)){
		fprintf(out, "Log -Putting flags- field [%i][%i] (%i) - adj_danger = %i, adj_flags = %i\n",field->tiles[i][j].mines_around, i, j, adj_danger, n_o_adjucent_flags(field, i, j));
		put_flags_around(field, i, j);
	}
	else if((field->tiles[i][j].mines_around - n_o_adjucent_flags(field, i, j) - adj_danger/100) == 0){
		fprintf(out, "Log -Revealing- field [%i][%i] (%i) - adj_danger = %i, adj_flags = %i\n",field->tiles[i][j].mines_around, i, j, adj_danger, n_o_adjucent_flags(field, i, j));
		byuser = 0;
		field->reveal_near(i, j);
		byuser = 1;
	}
	
		
}
