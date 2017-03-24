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

cell_stct * world_map(cell_stct * cell_list, int runs, int world_size);

cell_stct * next_world_gen(cell_stct *list, int world_size);

void generate_relative_coords(int pos_index, int *coords, int world_size);

Position belongs_to_diamond(int *coord_dif);

Position get_neighbour_pos(int distance, int coord_dif[3]);

void get_comp_coords(int near_border, int world_size, int xcomp, int ycomp, int zcomp, int x, int y, int z, int *coords );

void set_neighbour(cell_stct *current, cell_stct *neighbour, Position pos);

void clear_map(cell_stct *list);

#endif
