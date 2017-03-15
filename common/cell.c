#include "cell.h"

cell_tmp * add_cell_tmp(cell_tmp * ptr, int x , int y , int z){
  cell_tmp * new = (cell_tmp *) malloc(sizeof(cell_tmp));
  new->ptr = (cell_stct *) malloc(sizeof(cell_stct));
  new->ptr->x = x;
  new->ptr->y = y;
  new->ptr->z = z;
  new->next = ptr;
  return new;
}
