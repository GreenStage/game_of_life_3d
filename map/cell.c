#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cell.h"

typedef enum state {dead, alive, undefined} State;
typedef struct _pos_ { int x,y,z;} pos_;

struct cell_{
  pos_ pos;
  struct cell_ *first_neighbors[6]; /*INDEX ORDER -> FRONT, BACK, RIGHT, LEFT, UP, DOWN*/
  struct cell_ *second_neighbors[18]; /*INDEX ORDER -> U1_L1, U1_R1, U1_F1, U1_B1, D1_L1, D1_R1, D1_F1, D1_B1, B1_L1, B1_R1, F1_L1, F1_R1*/
  State state;
  State next_state;
  struct cell_ * next;
};

bool cell_exists(cell_ptr ptr) {  return (ptr)? true : false; }

cell_ptr cell_get_next(cell_ptr cell) { return cell->next; }
cell_ptr cell_get_next_state(cell_ptr cell) { return cell->next_state; }
cell_ptr cell_get_first_neighbor(cell_ptr cell, int i) { return cell->first_neighbors[i]; }
cell_ptr cell_get_second_neighbor(cell_ptr cell, int i) { return cell->second_neighbors[i]; }

void cell_add_first_neighbor(cell_ptr cell, int i, cell_ptr neigh){  cell->first_neighbors[i] = neigh; }

void cell_update_state(cell_ptr cell) {
  cell->state = cell->next_state;
  cell->next_state = undefined;
}


cell_ptr insert_new_cell( cell_ptr ptr, pos_ pos){

  cell_ptrnew_cell = (cell_stct*) malloc(sizeof(cell_stct));
  memset(new_cell,0,sizeof(cell_stct));

  new_cell->pos.x = pos.x;
  new_cell->pos.y = pos.y;
  new_cell->pos.z = pos.z;
  new_cell->state = alive;
  new_cell->next_state = alive;
  new_cell->next = ptr;

  return new_cell;
}

cell_ptr cell_remove_next(cell_ptr prev,cell_ptr cell){
  cell_ptr aux = cell->next;
  if ( prev != NULL ) prev->next = aux;
  free(cell);
  return aux ;
}

bool cell_will_spawn(int neighbors){
  if( neighbors == 2 || neighbors == 3 ){
    return true;
  }
  else return false;
}

void cell_find_next_state(cell_ptr cell,int neighbors){
  State current_state = cell_ptr->state;
  if(neighbors >= 2 && neighbors <= 4){
    cell_ptr->next_state = alive;
  }
  else cell_ptr->next_state = dead;

#ifdef DEBUG
    printf("Cell (%d,%d,%d) next state is %d\n",cell->pos.x,cell->pos.y,cell->pos.z,cell->next_state);
#endif

}

Position cell_belongs_to_diamond(cell_ptr cell, cell_ptr center){
  int distance;
  Position pos = NONE;
  int coord_dif[3] = {cell->pos.x - center->pos.x, cell->pos.y - center->pos.y, cell->pos.z - center->pos.z};

  if((distance = abs(coord_dif[0]) + abs(coord_dif[1]) + abs(coord_dif[2])) == 1 || distance == 2)
    pos = get_neighbour_pos(distance, coord_dif);
  else
    pos = NONE;

  return pos;
}

pos_ get_absolute(cell_ptr cell, int relative_position){
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

int cell_set_neighbors(cell_ptr cell1, cell_ptr cell,Position pos){
  int first_neighbors_ctr = 0;
  switch(pos){
    case NONE:
      break;

    case FRONT:
      first_neighbors_ctr++;
      cell1->first_neighbors[0] = cell2;
      cell2->first_neighbors[1] = cell1;
      break;

    case BACK:
      first_neighbors_ctr++;
      cell1->first_neighbors[1] = cell2;
      cell2->first_neighbors[0] = cell1;
      break;

    case RIGHT:
      first_neighbors_ctr++;
      cell1->first_neighbors[2] = cell2;
      cell2->first_neighbors[3] = cell1;
      break;

    case LEFT:
      first_neighbors_ctr++;
      cell1->first_neighbors[3] = cell2;
      cell2->first_neighbors[2] = cell1;
      break;

    case UP:
      first_neighbors_ctr++;
      cell1->first_neighbors[4] = cell2;
      cell2->first_neighbors[5] = cell1;
      break;

    case DOWN:
      first_neighbors_ctr++;
      cell1->first_neighbors[5] = cell2;
      cell2->first_neighbors[4] = cell1;
      break;

    case U1_L1:
      cell1->second_neighbors[0] = cell2;
      cell2->second_neighbors[1] = cell1;
      break;

    case U1_R1:
      cell1->second_neighbors[1] = cell2;
      cell2->second_neighbors[0] = cell1;
      break;

    case U1_F1:
      cell1->second_neighbors[2] = cell2;
      cell2->second_neighbors[3] = cell1;
      break;

    case U1_B1:
      cell1->second_neighbors[3] = cell2;
      cell2->second_neighbors[2] = cell1;
      break;

    case D1_L1:
      cell1->second_neighbors[4] = cell2;
      cell2->second_neighbors[5] = cell1;
      break;

    case D1_R1:
      cell1->second_neighbors[5] = cell2;
      cell2->second_neighbors[4] = cell1;
      break;

    case D1_F1:
      cell1->second_neighbors[6] = cell2;
      cell2->second_neighbors[7] = cell1;
      break;

    case D1_B1:
      cell1->second_neighbors[7] = cell2;
      cell2->second_neighbors[6] = cell1;
      break;

    case B1_L1:
      cell1->second_neighbors[8] = cell2;
      cell2->second_neighbors[9] = cell1;
      break;

    case B1_R1:
      cell1->second_neighbors[9] = cell2;
      cell2->second_neighbors[8] = cell1;
      break;


    case F1_L1:
      cell1->second_neighbors[10] = cell2;
      cell2->second_neighbors[11] = cell1;
      break;

    case F1_R1:
      cell1->second_neighbors[11] = cell2;
      cell2->second_neighbors[10] = cell1;
      break;

    case F2:
      cell1->second_neighbors[12] = cell2;
      cell2->second_neighbors[13] = cell1;
      break;

    case B2:
      cell1->second_neighbors[13] = cell2;
      cell2->second_neighbors[12] = cell1;
      break;

    case R2:
      cell1->second_neighbors[14] = cell2;
      cell2->second_neighbors[15] = cell1;
      break;

    case L2:
      cell1->second_neighbors[15] = cell2;
      cell2->second_neighbors[14] = cell1;
      break;

    case U2:
      cell1->second_neighbors[16] = cell2;
      cell2->second_neighbors[17] = cell1;
      break;
    case D2:
      cell1->second_neighbors[17] = cell2;
      cell2->second_neighbors[16] = cell1;
      break;

    default:
#ifdef DEBUG
      printf("ERROR NOW: Cell (%d,%d,%d) is <%d> of cell (%d,%d,%d)\n",cell2->pos.x,cell2->pos.y,cell2->pos.z,pos,cell1->pos.x,cell1->pos.y,cell1->pos.z);
#endif
      error_exit("Error: Invalid Position when building World ()", 18);
      break;
  }
#ifdef DEBUG
        if( pos != 0) printf("Cell (%d,%d,%d) is <%d> of cell (%d,%d,%d)\n",cell2->pos.x,cell2->pos.y,cell2->pos.z,pos,cell1->pos.x,cell1->pos.y,cell1->pos.z);
#endif
  return first_neighbors_ctr;
}

#ifdef DEBUG
void cell_list_print(cell_ptr ptr){
  cell_ptr aux;
  printf("____________________\n");
  for(aux = ptr; aux != NULL; aux = aux->next)
    printf("(%d,%d,%d) is %s\n", aux->pos.x,aux->pos.y,aux->pos.z,state_to_str(aux->state) );
}

void cell_will_spawn_alert(cell_ptr ptr){
  printf("Cell will spawn at (%d,%d,%d)",ptr->pos.x,ptr->pos.y,ptr->pos.z);
}
#endif /*DEBUG*/
