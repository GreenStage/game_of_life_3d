#ifndef CELLHEADER
#define CELLHEADER
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defs.h"

/*TYPES*/
typedef enum state{dead, alive, undefined}State;

typedef struct cell_{
  int x,y,z;
  int visited;
  struct cell_ *first_neighbors[6];
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
