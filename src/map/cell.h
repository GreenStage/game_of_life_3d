#ifndef CELL_HEADER
#define CELL_HEADER

#include "../common/defs.h"
#define state_to_str(STATE) ( (STATE) == (0) ? ("Dead") : ("Alive") )

/*TYPES*/
typedef struct  cell_ * cell_ptr;
typedef enum state {dead, alive, undefined} State;
/*FUNCTIONS*/

/*OPERATORS*/
cell_ptr insert_new_cell( cell_ptr ptr, pos_ pos,int max_pos);
cell_ptr cell_remove_next(cell_ptr prev,cell_ptr  cell);
cell_ptr cell_order(cell_ptr h);
void cell_free_list(cell_ptr head);
void cell_reset_neighbors(cell_ptr cell);
void cell_update_state(cell_ptr cell);
void cell_find_next_state(cell_ptr cell,int neighbors);
void cell_set_neighbors(cell_ptr cell1, cell_ptr cell2,int index);
void cell_add_neighbor(cell_ptr cell, int i, cell_ptr neigh);
void cell_list_print(cell_ptr ptr, FILE * file);

/*EVALUATING FUNCTIONS*/
bool cell_exists(cell_ptr ptr);
bool cell_will_spawn(int neighbors);

/*FETCHING FUNCTIONS*/
pos_ cell_get_pos(cell_ptr cell);
int cell_get_index_by_relative(relative_position pos);
int  cell_get_diamond_index(cell_ptr cell1, cell_ptr ref, pos_ xydif, int map_size, mirror near_border);
pos_ cell_get_absolute_pos(cell_ptr cell, int relative_position, int max_pos);
relative_position cell_get_relative_by_index(int index);
relative_position cell_get_relative_to_neighbor(relative_position pos_to_cell, relative_position pos_to_neighbor);
mirror cell_get_near_border(cell_ptr cell);
State cell_get_state(cell_ptr cell);
State cell_get_next_state(cell_ptr cell);
cell_ptr cell_get_next(cell_ptr cell);
cell_ptr cell_get_neighbor(cell_ptr cell, int i);
cell_ptr cell_list_update_state(cell_ptr cell);

int * list_to_array(cell_ptr head, int * arraySize, int size);
cell_ptr arrayToList(int x,int y,int * z, int buffSize, int max_pos, int startIndex);

/*DEBUGGING FUNCTIONS*/
#ifdef DEBUG

void cell_will_spawn_alert(cell_ptr ptr, cell_ptr owner);
#endif /*DEBUG*/

#endif
