#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "world.h"
#include "../common/error.h"
/*N PRECISAS GUARDAR SEGUNDOS VIZINHOS*/
world_stct * init_world( int size){
  world_stct * new_world = (world_stct* ) malloc(sizeof(world_stct));
  new_world->size = size;
  new_world->cell_list = NULL;

  return new_world;
}

<<<<<<< HEAD
void world_map(cell_stct * cell_list){
  int first_neighbors_ctr, i, j;
  int neighbor_neigh[6], count_n_neigh;
  cell_stct * aux1, *aux2;
  Position pos;
=======
world_stct * world_map(world_stct * world){
	int v, a, l;
  int size = world->size;
  cell_ptr cell_list = world->cell_list;
  cell_ptr aux;
  int first_neighbors_ctr, i, j, k, index;
  int neighbor_neigh[5], count_n_neigh;
  cell_ptr aux1, aux2;
  relative_position  pos;
  pos_ pos_aux;
  mirror border;
#ifdef DEBUG
  int counter = 0;
#endif
>>>>>>> b5d00a09313bd947385842343e2d957867d22d9b

  for( a = 0, aux1 = cell_list; aux1 != NULL; aux1 = cell_get_next(aux1) , a++ ){
    if(cell_get_state(aux1) != alive)
      continue;

<<<<<<< HEAD
    for(aux1 = cell_list; aux1 != NULL; aux1 = aux1->next){
      first_neighbors_ctr = 0;

=======
    first_neighbors_ctr = 0;
	for (i = 0; i < 6; i++) {
		aux = cell_get_first_neighbor(aux1, i);
		if (cell_exists(aux) && cell_get_state(aux) == alive) 
			first_neighbors_ctr++;
	}
>>>>>>> b5d00a09313bd947385842343e2d957867d22d9b
#ifdef DEBUG
      printf("Cell list processing at position %d\n", counter);
      counter++;
#endif

<<<<<<< HEAD
    for(aux2 = aux1->next; aux2 != NULL; aux2 = aux2->next){
      pos = belongs_to_diamond(aux1->x, aux1->y, aux1->z, aux2->x, aux2->y, aux2->z);
      switch(pos){

        case NONE:
          break;

        case FRONT:
          first_neighbors_ctr++;
          aux1->first_neighbors[0] = aux2;
          aux2->first_neighbors[1] = aux1;
          break;

        case BACK:
          first_neighbors_ctr++;
          aux1->first_neighbors[1] = aux2;
          aux2->first_neighbors[0] = aux1;
          break;

        case RIGHT:
          first_neighbors_ctr++;
          aux1->first_neighbors[2] = aux2;
          aux2->first_neighbors[3] = aux1;
          break;

        case LEFT:
          first_neighbors_ctr++;
          aux1->first_neighbors[3] = aux2;
          aux2->first_neighbors[2] = aux1;
          break;

        case UP:
          first_neighbors_ctr++;
          aux1->first_neighbors[4] = aux2;
          aux2->first_neighbors[5] = aux1;
          break;

        case DOWN:
          first_neighbors_ctr++;
          aux1->first_neighbors[5] = aux2;
          aux2->first_neighbors[4] = aux1;
          break;

        case U1_L1:
          aux1->second_neighbors[0] = aux2;
          aux2->second_neighbors[1] = aux1;
          break;

        case U1_R1:
          aux1->second_neighbors[1] = aux2;
          aux2->second_neighbors[0] = aux1;
          break;

        case U1_F1:
          aux1->second_neighbors[2] = aux2;
          aux2->second_neighbors[3] = aux1;
          break;

        case U1_B1:
          aux1->second_neighbors[3] = aux2;
          aux2->second_neighbors[2] = aux1;
          break;

        case D1_L1:
          aux1->second_neighbors[4] = aux2;
          aux2->second_neighbors[5] = aux1;
          break;

        case D1_R1:
          aux1->second_neighbors[5] = aux2;
          aux2->second_neighbors[4] = aux1;
          break;

        case D1_F1:
          aux1->second_neighbors[6] = aux2;
          aux2->second_neighbors[7] = aux1;
          break;

        case D1_B1:
          aux1->second_neighbors[7] = aux2;
          aux2->second_neighbors[6] = aux1;
          break;

        case B1_L1:
          aux1->second_neighbors[8] = aux2;
          aux2->second_neighbors[9] = aux1;
          break;

        case B1_R1:
          aux1->second_neighbors[9] = aux2;
          aux2->second_neighbors[8] = aux1;
          break;


        case F1_L1:
          aux1->second_neighbors[10] = aux2;
          aux2->second_neighbors[11] = aux1;
          break;

        case F1_R1:
          aux1->second_neighbors[11] = aux2;
          aux2->second_neighbors[10] = aux1;
          break;

        case F2:
          aux1->second_neighbors[12] = aux2;
          aux2->second_neighbors[13] = aux1;
          break;

        case B2:
          aux1->second_neighbors[13] = aux2;
          aux2->second_neighbors[12] = aux1;
          break;

        case R2:
          aux1->second_neighbors[14] = aux2;
          aux2->second_neighbors[15] = aux1;
          break;

        case L2:
          aux1->second_neighbors[15] = aux2;
          aux2->second_neighbors[14] = aux1;
          break;

        case U2:
          aux1->second_neighbors[16] = aux2;
          aux2->second_neighbors[17] = aux1;
          break;
        case D2:
          aux1->second_neighbors[17] = aux2;
          aux2->second_neighbors[16] = aux1;
          break;

        default:
#ifdef DEBUG
          printf("ERROR NOW: Cell (%d,%d,%d) is <%d> of cell (%d,%d,%d)\n",aux2->x,aux2->y,aux2->z,pos,aux1->x,aux1->y,aux1->z);
#endif
          error_exit("Error: Invalid Position when building World ()", 18);
          break;
      }
#ifdef DEBUG
        if( pos != 0) printf("Cell (%d,%d,%d) is <%d> of cell (%d,%d,%d)\n",aux2->x,aux2->y,aux2->z,pos,aux1->x,aux1->y,aux1->z);
#endif


  }/*inner loop*/
  aux1->next_state = cell_get_next_state(aux1->state,first_neighbors_ctr);
=======
    for(aux2 = cell_get_next(aux1); aux2 != NULL; aux2 = cell_get_next(aux2) ){
      border = cell_get_near_border(aux2);

      if ( -1  != (index = cell_get_diamond_index(aux2, aux1,world->size,near_none) ) )
        first_neighbors_ctr += cell_set_neighbors(aux1,aux2,index);

      if( border & near_x0 ){
        if( -1  != (index = cell_get_diamond_index(aux2, aux1,world->size,near_x0) ) )
          first_neighbors_ctr += cell_set_neighbors(aux1,aux2,index);
      }

      if( border & near_y0 ){
        if( -1  != (index = cell_get_diamond_index(aux2, aux1,world->size,near_y0) ) )
          first_neighbors_ctr += cell_set_neighbors(aux1,aux2,index);
      }

      if( border & near_z0 ){
        if( -1  != (index = cell_get_diamond_index(aux2, aux1,world->size,near_z0) ) )
          first_neighbors_ctr += cell_set_neighbors(aux1,aux2,index);
      }

      if( border & near_xmax ){
        if( -1  != (index = cell_get_diamond_index(aux2, aux1,world->size,near_xmax) ) )
          first_neighbors_ctr += cell_set_neighbors(aux1,aux2,index);
      }

      if( border & near_ymax ){
        if( -1  != (index = cell_get_diamond_index(aux2, aux1,world->size,near_ymax) ) )
          first_neighbors_ctr += cell_set_neighbors(aux1,aux2,index);
      }

      if( border & near_zmax ){
        if( -1  != (index = cell_get_diamond_index(aux2, aux1,world->size,near_zmax) ) )
          first_neighbors_ctr += cell_set_neighbors(aux1,aux2,index);
      }


	  if ( ( border & near_xmax ) && ( border & near_ymax ) ) {
		  if (-1 != (index = cell_get_diamond_index(aux2, aux1, world->size, near_xmax | near_ymax)))
			  if (cell_set_neighbors(aux1, aux2, index) != 0) exit(ERR_INVALID_GEN);
	  }

	  if ( (border & near_xmax) && (border & near_zmax) ) {
		  if (-1 != (index = cell_get_diamond_index(aux2, aux1, world->size, near_xmax | near_zmax)))
			  if (cell_set_neighbors(aux1, aux2, index) != 0) exit(ERR_INVALID_GEN);
	  }

	  if ((border & near_xmax) && (border & near_y0)) {
		  if (-1 != (index = cell_get_diamond_index(aux2, aux1, world->size, near_xmax | near_y0)))
			  if (cell_set_neighbors(aux1, aux2, index) != 0) exit(ERR_INVALID_GEN);
	  }

	  if ((border & near_xmax) && (border & near_z0)) {
		  if (-1 != (index = cell_get_diamond_index(aux2, aux1, world->size, near_xmax | near_z0)))
			  if (cell_set_neighbors(aux1, aux2, index) != 0) exit(ERR_INVALID_GEN);
	  }

	  if ((border & near_ymax) && (border & near_zmax)) {
		  if (-1 != (index = cell_get_diamond_index(aux2, aux1, world->size, near_ymax | near_zmax)))
			  if (cell_set_neighbors(aux1, aux2, index) != 0) exit(ERR_INVALID_GEN);
	  }

	  if ((border & near_ymax) && (border & near_x0)) {
		  if (-1 != (index = cell_get_diamond_index(aux2, aux1, world->size, near_ymax | near_x0)))
			  if (cell_set_neighbors(aux1, aux2, index) != 0) exit(ERR_INVALID_GEN);
	  }

	  if ((border & near_ymax) && (border & near_z0)) {
		  if (-1 != (index = cell_get_diamond_index(aux2, aux1, world->size, near_ymax | near_z0)))
			  if (cell_set_neighbors(aux1, aux2, index) != 0) exit(ERR_INVALID_GEN);
	  }

	  if ((border & near_zmax) && (border & near_x0)) {
		  if (-1 != (index = cell_get_diamond_index(aux2, aux1, world->size, near_zmax | near_x0)))
			  if (cell_set_neighbors(aux1, aux2, index) != 0) exit(ERR_INVALID_GEN);
	  }

	  if ((border & near_zmax) && (border & near_y0)) {
		  if (-1 != (index = cell_get_diamond_index(aux2, aux1, world->size, near_zmax | near_y0)))
			  if (cell_set_neighbors(aux1, aux2, index) != 0) exit(ERR_INVALID_GEN);
	  }

	  if ((border & near_x0) && (border & near_y0)) {
		  if (-1 != (index = cell_get_diamond_index(aux2, aux1, world->size, near_x0 | near_y0)))
			  if (cell_set_neighbors(aux1, aux2, index) != 0) exit(ERR_INVALID_GEN);
	  }

	  if ((border & near_x0) && (border & near_z0)) {
		  if (-1 != (index = cell_get_diamond_index(aux2, aux1, world->size, near_x0 | near_z0)))
			  if (cell_set_neighbors(aux1, aux2, index) != 0) exit(ERR_INVALID_GEN);
	  }
	  if ((border & near_y0) && (border & near_z0)) {
		  if (-1 != (index = cell_get_diamond_index(aux2, aux1, world->size, near_y0 | near_z0)))
			  if (cell_set_neighbors(aux1, aux2, index) != 0) exit(ERR_INVALID_GEN);
	  }
    }
>>>>>>> b5d00a09313bd947385842343e2d957867d22d9b

    cell_find_next_state(aux1,first_neighbors_ctr);
    for( i=0 ; i < 6 ; i++ ){ /*Dead neighbor should be born?*/
	     border = near_xmax << (i);
      count_n_neigh=1;
	  aux = cell_get_first_neighbor(aux1, i);
      if( !cell_exists( cell_get_first_neighbor(aux1,i) ) ){
        get_neighbors_by_key(neighbor_neigh,i);

        for(j = 0; j < 5; j++){
          aux = cell_get_second_neighbor(aux1, neighbor_neigh[j]);
          if(cell_exists( aux ) && cell_get_state(aux) == alive)
            count_n_neigh++;
        }
        if( cell_will_spawn(count_n_neigh) ){

          pos_aux = cell_get_absolute_pos(aux1,i,world->size - 1);
          cell_list = insert_new_cell(cell_list,pos_aux,world->size - 1) ;
          cell_add_first_neighbor(aux1,i,cell_list);

          for(k = 0; k < 5; k++){
            if(( cell_exists((aux = cell_get_second_neighbor(aux1, neighbor_neigh[k])) ) ) && cell_get_state(aux) == alive ){
			     v = cell_get_relative_to_neighbor(cell_get_relative_by_index(i), cell_get_relative_by_index(neighbor_neigh[k] + 6));
			     index = cell_get_index_by_relative(v);
		         cell_add_first_neighbor(aux, index ,cell_list);

            }
          }

#ifdef DEBUG
          cell_will_spawn_alert(cell_list);
#endif
        }
      }
    }

  }
  world->cell_list = cell_list;
  return world;
}


void get_neighbors_by_key(int retval[5], int key){
  memset(retval,-1, sizeof(int) * 5);
  switch(key){
    case 0: /*Front*/
      retval[0]=0; retval[1]=6; retval[2]=8;
      retval[3]=14; retval[4]=17;
      break;

    case 1: /*Back*/
       retval[0]=1; retval[1]=7; retval[2]=9;
       retval[3]=15; retval[4]=16;
      break;

    case 2: /*RIGHT*/
       retval[0]=2; retval[1]=7; retval[2]=8;
       retval[3]=11; retval[4]=12;
      break;

    case 3: /*LEFT*/
       retval[0]=3; retval[1]=6; retval[2]=9;
       retval[3]=10; retval[4]=13;
      break;

    case 4: /*UP*/
       retval[0]=4; retval[1]=10; retval[2]=12;
       retval[3]=14; retval[4]=16;
      break;

    case 5: /*DOWN*/
       retval[0]=5; retval[1]=11; retval[2]=13;
       retval[3]=15; retval[4]=17;
      break;
    default:
      break;
  }
  return;
}


world_stct * world_update_state(world_stct * world){
  cell_ptr head = world->cell_list;
  cell_ptr aux, aux2,neighbors;
  for(aux = head , aux2 = NULL; aux != NULL;){
	cell_reset_neighbors(aux);
    if(cell_get_next_state(aux) == dead) {
      if(head == aux){
        aux = cell_remove_next(aux2,aux);
        head = aux;
      }
      else aux = cell_remove_next(aux2,aux);
    }
    else{
      cell_update_state(aux);
      aux2 = aux;
      aux = cell_get_next(aux);
    }

  }
  world->cell_list = head;
  return world;
}

void clear_map(cell_stct *list){
  cell_stct *aux1, *aux2;

  for(aux1 = list; aux1 != NULL; aux1 = aux2){
    aux2 = aux1->next;
    free(aux1);
  }
}
