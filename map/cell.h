#ifndef CELL_HEADER
#define CELL_HEADER

#include "../common/defs.h"

/*TYPES*/

typedef struct  cell_ * cell_ptr;
typedef struct _pos_ { int x,y,z;} pos_;
typedef enum state {dead, alive, undefined} State;
cell_ptr cell_order(cell_ptr h);
/*FUNCTIONS*/
cell_ptr insert_new_cell( cell_ptr ptr, pos_ pos,int max_pos);
cell_ptr cell_remove_next(cell_ptr prev,cell_ptr  cell);

void cell_getz(cell_ptr ptr);
/*EVALUATING FUNCTIONS*/
bool cell_exists(cell_ptr ptr);
bool cell_will_spawn(int neighbors);

/*FETCHING FUNCTIONS*/
relative_position cell_get_relative_by_index(int index);
relative_position cell_get_relative_to_neighbor(relative_position pos_to_cell, relative_position pos_to_neighbor);
int cell_get_index_by_relative(relative_position pos);
mirror cell_get_near_border(cell_ptr cell);
relative_position  cell_get_neighbour_pos(int distance, int coord_dif[3]);
int  cell_get_diamond_index(cell_ptr cell1, cell_ptr ref, int map_size, mirror near_border);
int cell_get_index_by_pos(relative_position pos);
cell_ptr cell_get_next(cell_ptr cell);
State cell_get_state(cell_ptr cell);
State cell_get_next_state(cell_ptr cell);
cell_ptr cell_get_first_neighbor(cell_ptr cell, int i);
cell_ptr cell_get_second_neighbor(cell_ptr cell, int i);

/*UPDATING FUNCTIONS*/
void cell_update_state(cell_ptr cell);
void cell_find_next_state(cell_ptr cell,int neighbors);
int cell_set_neighbors(cell_ptr cell1, cell_ptr cell2,int index);
void cell_add_first_neighbor(cell_ptr cell, int i, cell_ptr neigh);
void cell_reset_neighbors(cell_ptr cell);
pos_ cell_get_absolute_pos(cell_ptr cell, int relative_position, int max_pos);

void cell_list_print(cell_ptr ptr, FILE * file);
#define state_to_str(STATE) ( (STATE) == (0) ? ("Dead") : ("Alive") )
/*DEBUGGING FUNCTIONS*/
#ifdef DEBUG

void cell_will_spawn_alert(cell_ptr ptr);
#endif /*DEBUG*/

#endif
