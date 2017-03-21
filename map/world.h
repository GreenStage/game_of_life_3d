#ifndef WORLD_HEADER
#define WORLD_HEADER
#include "../common/defs.h"
#include "cell.h"


typedef struct world_stct {
  int size;
  cell_ptr cell_list;
}world_stct;

/*INIT FUNCTION*/


world_stct * init_world( int size);

world_stct * world_map(world_stct * world);
void get_neighbors_by_key(int retval[5], int key);
world_stct * world_update_state(world_stct * world);
#endif
