#ifndef CELL_HEADER
#define CELL_HEADER

#include "../common/defs.h"

/*TYPES*/

typedef struct  cell_ * cell_ptr;

/*FUNCTIONS*/
cell_ptr  insert_new_cell( cell_ptr ptr, pos_ pos);
cell_ptr cell_remove_next(cell_ptr prev,cell_ptr  cell);


/*EVALUATING FUNCTIONS*/
bool cell_exists(cell_ptr ptr);
bool cell_will_spawn(int neighbors);

/*FETCHING FUNCTIONS*/
cell_ptr cell_get_next(cell_ptr cell);
cell_ptr cell_get_next_state(cell_ptr cell);
cell_ptr cell_get_first_neighbor(cell_ptr cell, int i);
cell_ptr cell_get_second_neighbor(cell_ptr cell, int i);

/*UPDATING FUNCTIONS*/
void cell_update_state(cell_ptr cell);
void cell_find_next_state(cell_ptr cell,int neighbors);
int cell_set_neighbors(cell_ptr cell1, cell_ptr cell,Position pos);
void cell_add_first_neighbor(cell_ptr cell, int i, cell_ptr neigh);

pos_ get_absolute(cell_ptr cell, int relative_position);
Position cell_belongs_to_diamond(cell_ptr cell, cell_ptr center);

/*DEBUGGING FUNCTIONS*/
#ifdef DEBUG
#define state_to_str(STATE) ( (STATE) == (0) ? ("Dead") : ("Alive") )
void cell_list_print(cell_ptr ptr);
void cell_will_spawn_alert(cell_ptr ptr);
#endif /*DEBUG*/

#endif
