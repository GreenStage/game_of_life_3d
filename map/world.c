#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "world.h"
#include "../common/error.h"

world_stct * init_world( int size){
  world_stct * new_world = (world_stct* ) malloc(sizeof(world_stct));
  new_world->size = size;
  new_world.cell_list = NULL;

  return new_world;
}

world_stct * world_map(world_stct * world){
  cell_ptr cell_list = world.cell_list;
  int first_neighbors_ctr, i, j  ;
  int next_state;
  int neighbor_neigh[6], count_n_neigh;
  cell_ptr aux1, *aux2;
  Position pos;

#ifdef DEBUG
    int counter = 0;
#endif

    for( aux1 = cell_list; aux1 != NULL; aux1 = cell_get_next(aux1) ){
      first_neighbors_ctr = 0;

#ifdef DEBUG
      printf("Cell list processing at position %d\n", counter);
      counter++;
#endif

    for( aux2 = cell_get_next(aux1); aux2 != NULL; aux2 = cell_get_next(aux2) ){
      pos = cell_belongs_to_diamond(aux2, aux1);
      first_neighbors_ctr = cell_set_neighbors(aux1,aux2,pos);
    }

    cell_find_next_state(aux1,first_neighbors_ctr);

    for( i=0 ; i < 6 ; i++ ){

      count_n_neigh=1;
      if( !cell_exists( cell_get_first_neighbor(aux1,i) ) ){
        get_neighbors_by_key(neighbor_neigh,i);

        for(j = 0; j < 5; j++){
          if(cell_exists( cell_get_second_neighbor(aux1, neighbor_neigh[j]) ) )
            count_n_neigh++;
        }

        if( cell_will_spawn(count_n_neigh) ){

          cell_list = insert_new_cell(cell_list,get_absolute(aux1,i)) ;
          cell_add_first_neighbor(aux1,i,cell_list);
#ifdef DEBUG
          cell_will_spawn_alert(cell_list);
#endif

        }
      }

    }
  }
  world.cell_list = cell_list;
  return world;
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

world_stct * world_update_state(world_stct * world){
  cell_ptr head = world.cell_list;
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
  world.cell_list = head;
  return world;
}
