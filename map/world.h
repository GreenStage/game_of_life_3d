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

<<<<<<< HEAD
void world_map(cell_stct * cell_list);
Position belongs_to_diamond(int xcomp, int ycomp, int zcomp, int x, int y, int z);
Position get_neighbour_pos(int distance, int coord_dif[3]);
void clear_map(cell_stct *list);

=======
world_stct * world_map(world_stct * world);
void get_neighbors_by_key(int retval[5], int key);
world_stct * world_update_state(world_stct * world);
>>>>>>> b5d00a09313bd947385842343e2d957867d22d9b
#endif
