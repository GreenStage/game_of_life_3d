#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cell.h"

cell_stct * insert_new_cell( cell_stct * ptr, int x , int y, int z){

  cell_stct *new_cell = (cell_stct*) malloc(sizeof(cell_stct));
  memset(new_cell,0,sizeof(cell_stct));

  new_cell->x = x;
  new_cell->y = y;
  new_cell->z = z;
  new_cell->visited = 0;
  new_cell->state = alive;
  new_cell->next_state = undefined;
  new_cell->next = ptr;

  return new_cell;
}

Position belongs_to_diamond(int xcomp, int ycomp, int zcomp, int x, int y, int z){
  int distance;
  Position pos = NONE;
  int coord_dif[3] = {x - xcomp, y - ycomp, z - zcomp};

  if((distance = abs(coord_dif[0]) + abs(coord_dif[1]) + abs(coord_dif[2])) == 1 || distance == 2)
    pos = get_neighbour_pos(distance, coord_dif);
  else
    pos = NONE;

  /*switch((distance = (abs(coord_dif[0]) + abs(coord_dif[1]) + abs(coord_dif[2]))){
    case 1: //is first neighbour
      pos = get_first_neighbour_pos(coord_dif);
      break;

    case 2: //is 2nd neighbour
      pos = get_neighbour_pos(distance, coord_dif);
      break;

    default:
      pos = NONE;
      break;
  }*/
  return pos;
}

Position get_neighbour_pos(int distance, int coord_dif[3]){
  int is_negative = 0, i;
  Position pos = NONE;

#ifdef DEBUG
  printf("coord_dif : [%d,%d,%d] , ",coord_dif[0],coord_dif[1],coord_dif[2]);
#endif

  for(i = 0; i< 3; i++){
    is_negative = (coord_dif[i] < 0) ? 2 : 0;

    if(coord_dif[i] == 2 || coord_dif[i] == -2){
      pos = (0b11 << (i * 4 + is_negative));
      break;
    }
    else if(coord_dif[i] == 1 || coord_dif[i] == -1){
      pos = pos | (0b01 << (i * 4 + is_negative));
      if(distance == 1)
        break;
    }
    else continue;
  }

  return pos;
}
