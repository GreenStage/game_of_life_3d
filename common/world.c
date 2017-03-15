#include "world.h"
#include "cell.h"

int get_start_post(int size, int pos){
  int retval;
  retval = pos - 1;
  if(retval < 0)
    retval = size + retval;
  return retval;
}

mini_world * miniworld_new(int size, int x,int y,int z){
  int i,j;
  mini_world * mini;

  mini = (mini_world * ) malloc(sizeof(mini_world));

  mini->size = 4;
  mini->destroy = miniworld_destroy;
  memset(&(mini.cells),0,sizeof(mini.cells));
  mini->next = NULL;
  mini->x = get_start_pos(size,x);
  mini->y = get_start_pos(size,y);
  mini->z = get_start_pos(size,z);
  mini->add_cell = mini_add_cell;
}


void miniworld_destroy(mini_world * world){

}
