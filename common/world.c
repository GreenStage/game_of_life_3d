#include "world.h"
#include "cell.h"

world_stct * world_init(){
  world_stct * world;
  world = (* world_stct) malloc(sizeof(world_stct));
  world->size = 64;
  world->destroy = world_destroy;
}

void world_destroy(world_stct * world){

}
