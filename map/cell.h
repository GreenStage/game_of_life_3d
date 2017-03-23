#ifndef CELL_HEADER
#define CELL_HEADER

#include "../common/defs.h"

/*TYPES*/
typedef enum state {dead = 0, alive = 1} State;

typedef struct cell_{
  int x,y,z;
  struct cell_ **first_neighbors; /*INDEX ORDER -> FRONT, BACK, RIGHT, LEFT, UP, DOWN*/
  struct cell_ **second_neighbors; /*INDEX ORDER -> U1_L1, U1_R1, U1_F1, U1_B1, D1_L1, D1_R1, D1_F1, D1_B1, B1_L1, B1_R1, F1_L1, F1_R1*/
  State state;
  State next_state;
  int visited;
  int near_border;
  struct cell_ * next;
} cell_stct;

/*FUNCTIONS*/
cell_stct * insert_new_cell(cell_stct *list, State st, int x , int y, int z, int world_size);

State cell_get_next_state(State current_state, int neighbours);

void get_neighbours_by_key(int retval[5], int key);

#endif
