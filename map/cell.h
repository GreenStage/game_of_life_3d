#ifndef CELL_HEADER
#define CELL_HEADER

#include "../common/defs.h"

/*TYPES*/
typedef enum state {dead, alive, undefined} State;
typedef struct _pos_ { int x,y,z;} pos_;
typedef struct cell_{
  pos_ pos;
  struct cell_ *first_neighbors[6]; /*INDEX ORDER -> FRONT, BACK, RIGHT, LEFT, UP, DOWN*/
  struct cell_ *second_neighbors[18]; /*INDEX ORDER -> U1_L1, U1_R1, U1_F1, U1_B1, D1_L1, D1_R1, D1_F1, D1_B1, B1_L1, B1_R1, F1_L1, F1_R1*/
  State state;
  State next_state;
  struct cell_ * next;
} cell_stct;

/*FUNCTIONS*/

pos_ get_absolute(cell_stct * cell, int relative_position);
cell_stct * cell_remove_next(cell_stct * prev,cell_stct * cell);
cell_stct * insert_new_cell( cell_stct * ptr, pos_ pos);
State cell_get_next_state(State current_state,int neighbors);
void get_neighbors_by_key(int retval[5], int key);

/*DEBUGGING FUNCTIONS*/
#ifdef DEBUG
#define state_to_str(STATE) ( (STATE) == (0) ? ("Dead") : ("Alive") )
void cell_list_print(cell_stct * ptr);
#endif
#endif
