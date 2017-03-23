  #include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cell.h"
#include "../common/error.h"

struct cell_{
  pos_ pos;
  mirror near_border;
  struct cell_ * first_neighbors[6]; /*INDEX ORDER -> FRONT, BACK, RIGHT, LEFT, UP, DOWN*/
  struct cell_ * second_neighbors[18]; /*INDEX ORDER -> F2, B2, R2, L2, U2, D2,
                                      F1_L1,B1_R1,F1_R1,B1_L1,U1_L1,D1_R1,U1_R1,D1_L1,U1_F1,D1_B1,U1_B1,D1_F1*/
  State state;
  State next_state;
  struct cell_ * next;
};

static const relative_position position_map[24] = { FRONT,BACK,RIGHT,LEFT,UP,DOWN, F2, B2, R2, L2, U2, D2,
                                      F1_L1,B1_R1,F1_R1,B1_L1,U1_L1,D1_R1,U1_R1,D1_L1,U1_F1,D1_B1,U1_B1,D1_F1};

relative_position cell_get_relative_by_index(int index) { return position_map[index]; }

int cell_get_index_by_relative(relative_position pos) {
	switch (pos) {
	case FRONT:
		return 0;
		break;
	case BACK:
		return 1;
		break;
	case RIGHT:
		return 2;
		break;
	case LEFT:
		return 3;
		break;
	case UP:
		return 4;
		break;
	case DOWN:
		return 5;
		break;
	case F2:
		return 6;
		break;
	case B2:
		return 7;
		break;
	case R2:
		return 8;
		break;
	case L2:
		return 9;
		break;
	case U2:
		return 10;
		break;
	case D2:
		return 11;
		break;
	case F1_L1:
		return 12;
		break;
	case B1_R1:
		return 13;
		break;
	case F1_R1:
		return 14;
		break;
	case B1_L1:
		return 15;
		break;
	case U1_L1:
		return 16;
		break;
	case D1_R1:
		return 17;
		break;
	case U1_R1:
		return 18;
		break;
	case D1_L1:
		return 19;
		break;
	case U1_F1:
		return 20;
		break;
	case D1_B1:
		return 21;
		break;
	case U1_B1:
		return 22;
		break;
	case D1_F1:
		return 23;
		break;

	default:
		return -1;
		break;
	}
}
relative_position cell_get_relative_to_neighbor(relative_position pos_to_cell, relative_position pos_to_neighbor){
	relative_position retval = NONE;


	if (pos_to_neighbor % 3 == 0) { /*F2,B2,R2,L2,U2,D2*/
		if (pos_to_neighbor == F2)		return BACK;
		else if (pos_to_neighbor == B2) return FRONT;
		else if (pos_to_neighbor == R2) return LEFT;
		else if (pos_to_neighbor == L2) return RIGHT;
		else if (pos_to_neighbor == U2) return DOWN;
		else if (pos_to_neighbor == D2) return UP;
	}

	else {
		retval = pos_to_neighbor ^ pos_to_cell;
	}
	if (retval == RIGHT || retval == FRONT || retval == UP) retval <<= 2;
	else if(retval == LEFT || retval == BACK || retval == DOWN) retval >>= 2;
	else retval = NONE;
	return retval;
}
void cell_getz(cell_ptr ptr){
  printf ("Z == %d\n", ptr->pos.z);
}
bool cell_exists(cell_ptr ptr) {  return (ptr != NULL)? 1 : 0; }
mirror cell_get_near_border(cell_ptr cell) { return cell->near_border;}
cell_ptr cell_get_next(cell_ptr cell) { return cell->next; }
State cell_get_state(cell_ptr cell) { return cell->state; }
State cell_get_next_state(cell_ptr cell) { return cell->next_state; }
cell_ptr cell_get_first_neighbor(cell_ptr cell, int i) { return cell->first_neighbors[i]; }
cell_ptr cell_get_second_neighbor(cell_ptr cell, int i) { return cell->second_neighbors[i]; }
mirror cell_is_near_border(cell_ptr cell) { return cell->near_border;}
void cell_add_first_neighbor(cell_ptr cell, int i, cell_ptr neigh){  cell->first_neighbors[i] = neigh; }

void cell_update_state(cell_ptr cell) {
  cell->state = cell->next_state;
  cell->next_state = undefined;
}


cell_ptr insert_new_cell( cell_ptr ptr, pos_ pos, int max_pos){
  mirror near_border = near_none;

  cell_ptr new_cell = (cell_ptr) malloc(sizeof( struct cell_));
  memset(new_cell,0,sizeof(struct cell_));
  memset(new_cell->first_neighbors,0,sizeof(struct cell_ *) * 6);
  memset(new_cell->second_neighbors,0,sizeof(struct cell_ *) * 18);
  new_cell->pos.x = pos.x;
  new_cell->pos.y = pos.y;
  new_cell->pos.z = pos.z;
  new_cell->state = dead;
  new_cell->next_state = alive;
  new_cell->next = ptr;

  if( max_pos - pos.x < 2 )
    near_border |= near_xmax;
  if( pos.x < 2 )
    near_border |= near_x0;

  if( max_pos - pos.y < 2 )
    near_border |= near_ymax;
  if( pos.y < 2 )
    near_border |= near_y0;

  if( max_pos - pos.z < 2 )
    near_border |= near_zmax;
  if( pos.z < 2 )
    near_border |= near_z0;

  new_cell->near_border = near_border;
  return new_cell;
}

cell_ptr cell_remove_next(cell_ptr prev,cell_ptr cell){
  cell_ptr aux = cell->next;
  if ( prev != NULL ) prev->next = aux;

  free(cell);
  return aux ;
}
void cell_reset_neighbors(cell_ptr cell) {
	memset(cell->first_neighbors, 0, sizeof(cell_ptr) * 6);
	memset(cell->second_neighbors, 0, sizeof(cell_ptr) * 18);
}

bool cell_will_spawn(int neighbors){
  if( neighbors == 2 || neighbors == 3 ){
    return true;
  }
  else return false;
}

void cell_find_next_state(cell_ptr cell,int neighbors){
  if(neighbors >= 2 && neighbors <= 4){
    cell->next_state = alive;
  }
  else cell->next_state = dead;

#ifdef DEBUG
    printf("Cell (%d,%d,%d) next state is %d\n",cell->pos.x,cell->pos.y,cell->pos.z,cell->next_state);
#endif

}

int  cell_get_diamond_index(cell_ptr cell1, cell_ptr ref, int map_size,mirror near_border){
  int distance,is_negative = 0, i;
  int retval = -1;
  pos_ pos = cell1->pos;
  int coord_dif[3];

  if (near_border & near_x0)	pos.x += map_size ;
  if (near_border & near_xmax) pos.x -= map_size ;
  
  if (near_border & near_y0) 	pos.y += map_size;
  if (near_border & near_ymax) pos.y -= map_size;
  
  if (near_border & near_z0) 	pos.z += map_size;
  if (near_border & near_zmax) pos.z -= map_size;

  coord_dif[0] = pos.x - ref->pos.x;
  coord_dif[1] = pos.y - ref->pos.y;
  coord_dif[2] = pos.z - ref->pos.z;

  if ((distance = abs(coord_dif[0]) + abs(coord_dif[1]) + abs(coord_dif[2])) != 1 && distance != 2)
	  return -1;

#ifdef DEBUG
  printf("coord_dif : [%d,%d,%d]\n ", coord_dif[0], coord_dif[1], coord_dif[2]);
#endif

  if (coord_dif[0] == 1 && coord_dif[1] == 0 && coord_dif[2] == 0) retval = 0;
  else if (coord_dif[0] == -1 && coord_dif[1] == 0 && coord_dif[2] == 0) retval = 1;
  else if (coord_dif[0] == 0 && coord_dif[1] == 1 && coord_dif[2] == 0) retval = 2;
  else if (coord_dif[0] == 0 && coord_dif[1] == -1 && coord_dif[2] == 0) retval = 3;
  else if (coord_dif[0] == 0 && coord_dif[1] == 0 && coord_dif[2] == 1) retval = 4;
  else if (coord_dif[0] == 0 && coord_dif[1] == 0 && coord_dif[2] == -1) retval = 5;

  else if (coord_dif[0] == 2 && coord_dif[1] == 0 && coord_dif[2] == 0) retval = 6;
  else if (coord_dif[0] == -2 && coord_dif[1] == 0 && coord_dif[2] == 0) retval = 7;
  else if (coord_dif[0] == 0 && coord_dif[1] == 2 && coord_dif[2] == 0) retval = 8;
  else if (coord_dif[0] == 0 && coord_dif[1] == -2 && coord_dif[2] == 0) retval = 9;
  else if (coord_dif[0] == 0 && coord_dif[1] == 0 && coord_dif[2] == 2) retval = 10;
  else if (coord_dif[0] == 0 && coord_dif[1] == 0 && coord_dif[2] == -2) retval = 11;
  else if (coord_dif[0] == 1 && coord_dif[1] == -1 && coord_dif[2] == 0) retval = 12;
  else if (coord_dif[0] == -1 && coord_dif[1] == 1 && coord_dif[2] == 0) retval = 13;
  else if (coord_dif[0] == 1 && coord_dif[1] == 1 && coord_dif[2] == 0) retval = 14;
  else if (coord_dif[0] == -1 && coord_dif[1] == -1 && coord_dif[2] == 0) retval = 15;
  else if (coord_dif[0] == 0 && coord_dif[1] == -1 && coord_dif[2] == 1) retval = 16;
  else if (coord_dif[0] == 0 && coord_dif[1] == 1 && coord_dif[2] == -1) retval = 17;
  else if (coord_dif[0] == 0 && coord_dif[1] == 1 && coord_dif[2] == 1) retval = 18;
  else if (coord_dif[0] == 0 && coord_dif[1] == -1 && coord_dif[2] == -1) retval = 19;
  else if (coord_dif[0] == 1 && coord_dif[1] == 0 && coord_dif[2] == 1) retval = 20;
  else if (coord_dif[0] == -1 && coord_dif[1] == 0 && coord_dif[2] == -1) retval = 21;
  else if (coord_dif[0] == -1 && coord_dif[1] == 0 && coord_dif[2] == 1) retval = 22;
  else if (coord_dif[0] == 1 && coord_dif[1] == 0 && coord_dif[2] == -1) retval = 23;
  else retval = -1;
  return retval;
}

pos_ cell_get_absolute_pos(cell_ptr cell, int relative_position, int max_pos){

  pos_ retval = cell->pos;

  switch(relative_position){
    case 0:
      retval.x ++;
      if(retval.x > max_pos)
        retval.x = 0;
      break;
    case 1:
      retval.x --;
      if(retval.x == -1)
        retval.x = max_pos;
      break;
    case 2:
      retval.y ++;
      if(retval.y > max_pos)
        retval.y = 0;
      break;
    case 3:
      retval.y --;
      if(retval.y == -1)
        retval.y = max_pos;
      break;
    case 4:
      retval.z ++;
      if(retval.z > max_pos)
        retval.z = 0;
      break;
    case 5:
      retval.z --;
      if(retval.z == -1)
        retval.z = max_pos;
      break;
    default:
      break;
  }
  return retval;
}
int cell_set_neighbors(cell_ptr cell1, cell_ptr cell2,int index){
  int first_neighbors_ctr = 0;

  #ifdef DEBUG
      printf("Cell (%d,%d,%d) is on index <%d> of cell (%d,%d,%d)\n",cell2->pos.x,cell2->pos.y,cell2->pos.z,index,cell1->pos.x,cell1->pos.y,cell1->pos.z);
  #endif
  if(index < 6){
    cell1->first_neighbors[index] = cell2;
    cell2->first_neighbors[(index % 2 == 0) ? index + 1 : index - 1] = cell1;
    first_neighbors_ctr++;
  }
  else{
    index = index - 6;
    cell1->second_neighbors[index] = cell2;
    cell2->second_neighbors[(index % 2 == 0) ? index + 1 : index - 1] = cell1;
  }


  return first_neighbors_ctr;
}
void cell_list_print(cell_ptr ptr, FILE * file){
  cell_ptr aux;
  for(aux = ptr; aux != NULL; aux = aux->next)
    fprintf(file,"(%d,%d,%d) is %s\n", aux->pos.x,aux->pos.y,aux->pos.z,state_to_str(aux->state) );
}
void cell_switch(cell_ptr c1, cell_ptr c2){
  struct cell_ aux;
  cell_ptr aux_next1 = c1->next;
  cell_ptr aux_next2 = c2->next;
  memcpy(&aux,c1,sizeof(struct cell_));
  memcpy(c1,c2,sizeof(struct cell_));
  memcpy(c2,&aux,sizeof(struct cell_));

  c1->next = aux_next1;
  c2->next = aux_next2;
}
cell_ptr cell_order(cell_ptr h){
  cell_ptr aux,aux1,aux2;
  cell_ptr smaller;
  for(aux = h; aux != NULL; aux = aux->next){

    smaller = aux;
    for(aux2 = aux->next; aux2 != NULL; aux2 = aux2->next ){
      if(aux2->pos.x < smaller->pos.x) smaller = aux2;
      else if (aux2->pos.x == smaller->pos.x){
        if(aux2->pos.y < smaller->pos.y) smaller = aux2;
        else if( aux2->pos.y == smaller->pos.y){
          if( aux2->pos.z < smaller->pos.z) smaller = aux2;
        }
      }
    }
    if( aux != smaller) cell_switch(aux,smaller);
  }
  return h;
}
#ifdef DEBUG

void cell_will_spawn_alert(cell_ptr ptr){
  printf("Cell will spawn at (%d,%d,%d)\n",ptr->pos.x,ptr->pos.y,ptr->pos.z);
}
#endif /*DEBUG*/
