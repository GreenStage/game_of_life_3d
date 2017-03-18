#ifndef CELL_HEADER
#define CELL_HEADER

#include "../common/defs.h"

/*TYPES*/
typedef enum state {dead, alive, undefined} State;

typedef struct cell_{
  int x,y,z;
  struct cell_ *first_neighbors[6]; /*INDEX ORDER -> FRONT, BACK, RIGHT, LEFT, UP, DOWN*/
  struct cell_ *second_neighbors[18]; /*INDEX ORDER -> U1_L1, U1_R1, U1_F1, U1_B1, D1_L1, D1_R1, D1_F1, D1_B1, B1_L1, B1_R1, F1_L1, F1_R1*/
  State state;
  State next_state;
  struct cell_ * next;
} cell_stct;

/*FUNCTIONS*/
cell_stct * insert_new_cell( cell_stct * ptr, int x , int y, int z);
State cell_get_next_state(State current_state,int neighbors);
void get_neighbors_by_key(int retval[5], int key);
#endif
