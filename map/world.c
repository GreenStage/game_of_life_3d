#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "world.h"
#include "../common/error.h"

world_stct * init_world( int size){
  world_stct * new_world = (world_stct* ) malloc(sizeof(world_stct));
  new_world->size = size;
  new_world->cell_list = NULL;

  return new_world;
}

world_stct * world_map(world_stct * world){
  int size = world->size;
  cell_ptr cell_list = world->cell_list;
  cell_ptr aux;
  int first_neighbors_ctr, i, j, k;
  int neighbor_neigh[6], count_n_neigh;
  cell_ptr aux1, aux2;
  relative_position  pos;
  pos_ pos_aux;
  mirror border;
#ifdef DEBUG
  int counter = 0;
#endif

  for( aux1 = cell_list; aux1 != NULL; aux1 = cell_get_next(aux1) ){
    if(cell_get_state(aux1) != alive)
      continue;

    first_neighbors_ctr = 0;
#ifdef DEBUG
      printf("Cell list processing at position %d\n", counter);
      counter++;
#endif

    for( aux2 = cell_get_next(aux1); aux2 != NULL; aux2 = cell_get_next(aux2) ){
      border = cell_get_near_border(aux2);
      if ( NONE !=  ( pos = cell_get_relative_position(aux2, aux1,world->size,near_none) ) )
        first_neighbors_ctr += cell_set_neighbors(aux1,aux2,pos);

      if( border & near_x0 ){
        if( NONE !=  ( pos = cell_get_relative_position(aux2, aux1,world->size,near_x0) ) )
          first_neighbors_ctr += cell_set_neighbors(aux1,aux2,pos);
      }

      if( border & near_y0 ){
        if( NONE !=  ( pos = cell_get_relative_position(aux2, aux1,world->size,near_y0) ) )
          first_neighbors_ctr += cell_set_neighbors(aux1,aux2,pos);
      }

      if( border & near_z0 ){
        if( NONE !=  ( pos = cell_get_relative_position(aux2, aux1,world->size,near_z0) ) )
          first_neighbors_ctr += cell_set_neighbors(aux1,aux2,pos);
      }

      if( border & near_xmax ){
        if( NONE !=  ( pos = cell_get_relative_position(aux2, aux1,world->size,near_xmax) ) )
          first_neighbors_ctr += cell_set_neighbors(aux1,aux2,pos);
      }

      if( border & near_ymax ){
        if( NONE !=  ( pos = cell_get_relative_position(aux2, aux1,world->size,near_ymax) ) )
          first_neighbors_ctr += cell_set_neighbors(aux1,aux2,pos);
      }

      if( border & near_zmax ){
        if( NONE !=  ( pos = cell_get_relative_position(aux2, aux1,world->size,near_zmax) ) )
          first_neighbors_ctr += cell_set_neighbors(aux1,aux2,pos);
      }

    }

    cell_find_next_state(aux1,first_neighbors_ctr);

    for( i=0 ; i < 6 ; i++ ){ /*Dead neighbor should be born?*/

      count_n_neigh=1;

      if( !cell_exists( cell_get_first_neighbor(aux1,i) ) ){
        get_neighbors_by_key(neighbor_neigh,i);

        for(j = 0; j < 5; j++){
          if(cell_exists( cell_get_second_neighbor(aux1, neighbor_neigh[j]) ) )
            count_n_neigh++;
        }
        if( cell_will_spawn(count_n_neigh) ){
          pos_aux = cell_get_absolute_pos(aux1,i,world->size - 1);
          cell_list = insert_new_cell(cell_list,pos_aux,world->size - 1) ;
          cell_add_first_neighbor(aux1,i,cell_list);

          for(k = 0; k < 5; k++){

            if(( cell_exists((aux = cell_get_second_neighbor(aux1, neighbor_neigh[k])) ) ) && cell_get_state(aux) == alive ){
              //printf("STATE: %d\n",cell_get_state(aux));
              printf("index: %d\n",cell_get_relative_position(cell_list,aux,world->size,cell_get_near_border(cell_list) ));

              cell_add_first_neighbor(aux, cell_get_index_by_pos( cell_get_relative_position(cell_list,aux,world->size, cell_get_near_border(cell_list)) ) ,cell_list);
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
  cell_ptr aux, aux2;

  for(aux = head , aux2 = NULL; aux != NULL;){

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
