#ifndef WORLD_HEADER
#define WORLD_HEADER
#include "../common/defs.h"
#include "cell.h"


typedef struct world_stct {
  int size;
  void * cell_list;
}world_stct;

world_stct * init_world( int size);

void world_map(cell_stct * cell_list);
Position belongs_to_diamond(int xcomp, int ycomp, int zcomp, int x, int y, int z);
Position get_neighbour_pos(int distance, int coord_dif[3]);
#endif
