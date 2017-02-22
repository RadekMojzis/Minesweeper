
#ifndef BOT_INCLUDED
#define BOT_INCLUDED

#include "minefield.h"

class coordinates{
  public:
    int x;
    int y;
};

void use_bot(minefield* field);

bool is_solvable(minefield* field, int start_x, int start_y);

coordinates reveal_empty(minefield *field);

coordinates put_flags(minefield *field);

#endif 
