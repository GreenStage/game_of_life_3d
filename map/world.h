#ifndef WORLD_HEADER
#define WORLD_HEADER
#include "../common/defs.h"
#include "cell.h"


typedef struct world_stct {
  int size;
  cell_ptr ** cell_matrix;
}world_stct;

/*INIT FUNCTION*/
void world_order(world_stct * world);
void world_get_next_gen(world_stct * world);
void world_print(world_stct * world,FILE * file);
cell_ptr  world_get_next_list(world_stct * world,int x, int y,int it);
world_stct * init_world( int size);
void world_add_cell( world_stct * world, pos_ pos);
void world_update_gen(world_stct * world);
void world_map(world_stct * world);
void get_neighbors_by_key(int retval[5], int key);
world_stct * world_update_state(world_stct * world);
#endif
