#include "cell.h"

cell_stct * new_cell( cell_stct * ptr, int x , int y, int z){
  cell_stct * new_cell;
  new_cell = (cell_stct*) malloc(sizeof(cell_stct));
  memset(new_cell,0,sizeof(cell_stct));
  new_cell->x = x;
  new_cell->y = y;
  new_cell->z = z;
  new_cell->next = ptr;
  return new_cell;
}
