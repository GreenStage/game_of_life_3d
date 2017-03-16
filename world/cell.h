#ifndef CELLHEADER
#define CELLHEADER

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../common/defs.h"

/*TYPES*/
typedef enum state {dead, alive, undefined} State;

typedef struct cell_{
  int x,y,z;
  int visited;
  struct cell_ *first_neighbors[6]; /*INDEX ORDER -> FRONT, BACK, RIGHT, LEFT, UP, DOWN*/

  /*INDEX ORDER -> U1_L1, U1_R1, U1_F1, U1_B1, D1_L1, D1_R1, D1_F1, D1_B1, B1_L1, B1_R1, F1_L1, F1_R1
   *               F2, B2, R2, L2, U2, D2*/
  struct cell_ *second_neighbors[18];
  State state;
  State next_state;
  struct cell_ * next;
} cell_stct;

/*FUNCTIONS*/
cell_stct * insert_new_cell( cell_stct * ptr, int x , int y, int z);

Position belongs_to_diamond(int xcomp, int ycomp, int zcomp, int x, int y, int z);

Position get_first_neighbour_pos(int coord_dif[3]);

Position get_neighbour_pos( int distance, int coord_dif[3]);

#endif
