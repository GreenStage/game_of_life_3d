#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cell.h"

cell_stct * insert_new_cell(cell_stct *list, State st, int x , int y, int z, int world_size){
  cell_stct *new_cell = (cell_stct*) malloc(sizeof(cell_stct));
  memset(new_cell, 0, sizeof(cell_stct));

  new_cell->x = x;
  new_cell->y = y;
  new_cell->z = z;
  new_cell->state = st;
  new_cell->next_state = alive; /*default alive*/
  new_cell->visited = 0;

  if(x - world_size >= -2 || y - world_size >= -2 || z - world_size >= -2)
    new_cell->near_border = 1;
  else if(x <= 1 || y <= 1 || z <= 1)
    new_cell->near_border = 1;

  new_cell->first_neighbors = (cell_stct**) malloc(6*sizeof(cell_stct*));
  memset(new_cell->first_neighbors, 0, 6*sizeof(cell_stct*));

  new_cell->second_neighbors  = (cell_stct**) malloc(18*sizeof(cell_stct*));
  memset(new_cell->second_neighbors, 0, 18*sizeof(cell_stct*));

  new_cell->next = list;

  return new_cell;
}

State cell_get_next_state(State current_state, int neighbours){
  if( current_state == alive && neighbours >= 2 && neighbours <= 4){
    return alive;
  }
  else if( current_state == dead && (neighbours == 2 || neighbours == 3) ){
    return alive;
  }
  else return dead;
}

void get_neighbours_by_key(int retval[5], int key){

  switch(key){
    case 0: /*Front*/
      retval[0]=2; retval[1]=6; retval[2]=10;
      retval[3]=11; retval[4]=12;
      break;

    case 1:
       retval[0]=3; retval[1]=7; retval[2]=8;
       retval[3]=9; retval[4]=13;
      break;

    case 2:
       retval[0]=1; retval[1]=5; retval[2]=9;
       retval[3]=11; retval[4]=14;
      break;

    case 3:
       retval[0]=0; retval[1]=4; retval[2]=8;
       retval[3]=10; retval[4]=15;
      break;

    case 4:
       retval[0]=0; retval[1]=1; retval[2]=2;
       retval[3]=3; retval[4]=16;
      break;

    case 5:
       retval[0]=4; retval[1]=5; retval[2]=6;
       retval[3]=7; retval[4]=17;
      break;
    default:
      printf("get_neighbours_by_key ERROR\n\n");
      break;

  }

  return;
}
