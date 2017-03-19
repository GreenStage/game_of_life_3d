#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cell.h"

cell_stct * insert_new_cell( cell_stct * ptr, pos_ pos){

  cell_stct *new_cell = (cell_stct*) malloc(sizeof(cell_stct));
  memset(new_cell,0,sizeof(cell_stct));

  new_cell->pos.x = pos.x;
  new_cell->pos.y = pos.y;
  new_cell->pos.z = pos.z;
  new_cell->state = alive;
  new_cell->next_state = undefined;
  new_cell->next = ptr;

  return new_cell;
}
cell_stct * cell_remove_next(cell_stct * prev,cell_stct * cell){
  cell_stct * aux = cell->next;
  if ( prev != NULL ) prev->next = aux;
  free(cell);
  return aux ;
}
State cell_get_next_state(State current_state,int neighbors){
  if( current_state == alive && neighbors >= 2 && neighbors <= 4){
    return alive;
  }
  else if( current_state == dead && (neighbors == 2 || neighbors == 3) ){
    return alive;
  }
  else return dead;
}


void get_neighbors_by_key(int retval[5], int key){

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
    default:
       retval[0]=4; retval[1]=5; retval[2]=6;
       retval[3]=7; retval[4]=17;
      break;

  }
  return;
}

pos_ get_absolute(cell_stct * cell, int relative_position){
  pos_ retval = cell->pos;
  switch(relative_position){
    case 0:
      retval.x ++;
      break;
    case 1:
      retval.x --;
      break;
    case 2:
      retval.y ++;
      break;
    case 3:
      retval.y --;
      break;
    case 4:
      retval.z ++;
      break;
    case 5:
      retval.z --;
      break;
    default:
      break;
  }
  return retval;
}
#ifdef DEBUG
  void cell_list_print(cell_stct * ptr){
    cell_stct * aux;
    printf("____________________\n");
    for(aux = ptr; aux != NULL; aux = aux->next)
      printf("(%d,%d,%d) is %s\n", aux->pos.x,aux->pos.y,aux->pos.z,state_to_str(aux->state) );
  }
#endif
