#include "world.h"


world_stct * init( void (*cell_reate)() ){
  world_stct * new_world;
  new_world->add_cell = cell_create;
  new_world->set_cell_neighbors =
  new_world->cell_ptr = NULL;
}

void world_map(world_stct * world){

}
