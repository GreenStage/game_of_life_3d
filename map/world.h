#ifndef WORLD_HEADER
#define WORLD_HEADER
#include "../common/defs.h"
#include "cell.h"


typedef struct world_stct {
  int size;
  cell_stct * cell_list;
}world_stct;

/*FUNCTIONS*/
world_stct * init_world( int size);

cell_stct * world_map(cell_stct * cell_list, int runs);

cell_stct * insert_new_cell(cell_stct *list, State st, int x , int y, int z);

cell_stct * next_world_gen(cell_stct *list);

void generate_coords(int *coords, int pos_index);

Position belongs_to_diamond(int xcomp, int ycomp, int zcomp, int x, int y, int z);

Position get_neighbour_pos(int distance, int coord_dif[3]);

void set_neighbour(cell_stct *current, cell_stct *neighbour, Position pos);

void clear_map(cell_stct *list);

#endif
