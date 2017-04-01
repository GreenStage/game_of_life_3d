#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "world.h"
#include "../common/error.h"


struct world_stct world_st;
/*Exclusive Functions*/
int get_coord(int a, int size){
  if(a >= size) a = a - size;
  else if(a < 0) a = size + a ;
  return a;
}



pos_ get_pos_by_it(int it) {
	pos_ retval;
	switch (it) {
		case 0:
			retval.x = 0; retval.y = 0;
			break;
		case 1:
			retval.x = 0; retval.y = 1;
			break;
		case 2:
			retval.x = 0; retval.y = -1;
			break;
		case 3:
			retval.x = 1; retval.y = 0;
			break;
		case 4:
			retval.x = -1; retval.y = 0;
			break;
		case 5:
			retval.x = 2; retval.y = 0;
			break;
		case 6:
			retval.x = -2; retval.y = 0;
			break;
		case 7:
			retval.x = 0; retval.y = 2;
			break;
		case 8:
			retval.x = 0; retval.y = -2;
			break;
		case 9:
			retval.x = 1; retval.y = 1;
			break;
		case 10:
			retval.x = 1; retval.y = -1;
			break;
		case 11:
			retval.x = -1; retval.y = 1;
			break;
		case 12:
			retval.x = -1; retval.y = -1;
			break;
	}
	return retval;
}

cell_ptr world_get_next_list(int x, int y,int it){
  switch(it){
    case 0 :
      return  world->cell_matrix[x][y];
      break;
    case 1 :
      return  world->cell_matrix[x][get_coord(y + 1,world->size)];
      break;
    case 2 :
      return  world->cell_matrix[x][get_coord(y - 1,world->size)];
      break;
    case 3 :
      return  world->cell_matrix[get_coord(x + 1,world->size)][y];
      break;
    case 4 :
      return  world->cell_matrix[get_coord(x - 1,world->size)][y];
      break;
    case 5 :
      return  world->cell_matrix[get_coord(x + 2,world->size)][y];
      break;
    case 6 :
      return  world->cell_matrix[get_coord(x - 2,world->size)][y];
      break;
    case 7 :
      return  world->cell_matrix[x][get_coord(y + 2,world->size)];
      break;
    case 8 :
      return  world->cell_matrix[x][get_coord(y - 2,world->size)];
      break;

    case 9 :
      return  world->cell_matrix[get_coord(x + 1,world->size)][get_coord(y + 1,world->size)];
      break;
    case 10 :
      return  world->cell_matrix[get_coord(x + 1,world->size)][get_coord(y - 1,world->size)];
      break;
    case 11 :
      return  world->cell_matrix[get_coord(x - 1,world->size)][get_coord(y + 1,world->size)];
      break;
    case 12 :
      return  world->cell_matrix[get_coord(x - 1,world->size)][get_coord(y - 1,world->size)];
      break;
    default:
      return NULL;
      break;
  }
}


void get_neighbors_by_key(int retval[5], int key){
  memset(retval,-1, sizeof(int) * 5);
  switch(key){
    case 0: /*Front*/
      retval[0]= 6 + 0; retval[1]= 6 + 6; retval[2]= 6 + 8;
      retval[3]= 6 + 14; retval[4]= 6 + 17;
      break;

    case 1: /*Back*/
       retval[0]= 6 + 1; retval[1]= 6 + 7; retval[2]= 6 + 9;
       retval[3]= 6 + 15; retval[4]= 6 + 16;
      break;

    case 2: /*RIGHT*/
       retval[0]= 6 + 2; retval[1]= 6 + 7; retval[2]= 6 + 8;
       retval[3]= 6 + 11; retval[4]= 6 + 12;
      break;

    case 3: /*LEFT*/
       retval[0]= 6 + 3; retval[1]= 6 + 6; retval[2]= 6 + 9;
       retval[3]= 6 + 10; retval[4]= 6 + 13;
      break;

    case 4: /*UP*/
       retval[0]= 6 + 4; retval[1]= 6 + 10; retval[2]= 6 + 12;
       retval[3]= 6 + 14; retval[4]= 6 + 16;
      break;

    case 5: /*DOWN*/
       retval[0]= 6 + 5; retval[1]= 6 + 11; retval[2]= 6 + 13;
       retval[3]= 6 + 15; retval[4]= 6 + 17;
      break;
    default:
      break;
  }
  return;
}
/********************/

void world_destroy(){
  int it,it2;
  for(it =  0; it < world->size; it++){
    for(it2 = 0; it2 < world->size; it2++){
        cell_free_list(world->cell_matrix[it][it2]);
    }
  }
}

void world_add_cell(pos_ pos){
  world->cell_matrix[pos.x][pos.y] = insert_new_cell(world->cell_matrix[pos.x][pos.y],pos, world->size -1);

  cell_update_state(world->cell_matrix[pos.x][pos.y]);

}

void world_map(){
  int it1, it2, it3;
  int first_neighbors_ctr, index;
  cell_ptr  aux1, aux2, aux3;
  mirror border;
#ifdef DEBUG
  int counter = 0;
#endif

  for(it1= 0, it2 = 0, aux1 = world->cell_matrix[0][0]; it1 < world->size;){

    while(!cell_exists(aux1)){
      if(it2 == world->size -1){
        it1 ++;
		it2 = 0;
        if(it1 == world->size) break;
      }
      else it2++;
      aux1 = world->cell_matrix[it1][it2];
    }
    if(it1 == world->size) break;

    first_neighbors_ctr = 0;

#ifdef DEBUG
      printf("Cell list processing at position %d\n", counter);
      counter++;
#endif
    border = near_none;

    for(it3 = 0, aux2 = world->cell_matrix[it1][it2]; it3 < 13 ; it3++ , aux2 = world_get_next_list(it1, it2, it3)){
      for(aux3 = aux2; aux3 != NULL; aux3 = cell_get_next(aux3)){
        border = cell_get_near_border(aux3);

		if (-1 != (index = cell_get_diamond_index(aux3, aux1, get_pos_by_it(it3), world->size,near_none)))
			cell_set_neighbors(aux1, aux3, index);

		if (-1 != ( index = cell_get_diamond_index(aux3, aux1, get_pos_by_it(it3), world->size, border & (near_zmax | near_z0))) )
			cell_set_neighbors(aux1, aux3, index);

      }
    }
    aux1 = cell_get_next(aux1);
  }
}


void world_get_next_gen(){
  int it1,it2;
  int it3, it4, it5;
  int index,v;
  int neighbors_count;
  int neighbor_neigh[5];
  cell_ptr aux,aux1,aux2;
  mirror border = near_none;
  pos_ pos_aux;
  for(it1= 0, it2 = 0, aux1 = world->cell_matrix[0][0]; it1 < world->size; aux1 = cell_get_next(aux1)){

	while (!cell_exists(aux1)) {
		if (it2 == world->size - 1) {
			it1++;
			it2 = 0;
			if (it1 == world->size) break;
		}
		else it2++;
		aux1 = world->cell_matrix[it1][it2];
	}
	if (it1 == world->size) break;
	if (cell_get_state(aux1) != alive) continue;

    for(neighbors_count = 0, it3 = 0; it3 < 6; it3 ++){
		aux2 = cell_get_neighbor(aux1, it3);
		if( cell_exists( aux2 ) && cell_get_state( aux2 ) == alive ) neighbors_count ++;
    }
    cell_find_next_state(aux1,neighbors_count);

    for( it3=0 ; it3 < 6 ; it3++ ){ /*Dead neighbor should be born?*/
	    border = near_xmax << (it3);
      neighbors_count = 1;

      if( !cell_exists( cell_get_neighbor(aux1,it3) ) ){
        get_neighbors_by_key(neighbor_neigh,it3);

        for(it4 = 0; it4 < 5; it4++){
          aux = cell_get_neighbor(aux1, neighbor_neigh[it4]);
          if(cell_exists( aux ) && cell_get_state(aux) == alive)
            neighbors_count++;
        }

        if( cell_will_spawn(neighbors_count) ){
          pos_aux = cell_get_absolute_pos(aux1,it3,world->size - 1);
          world->cell_matrix[pos_aux.x][pos_aux.y] = insert_new_cell(world->cell_matrix[pos_aux.x][pos_aux.y],pos_aux,world->size - 1) ;
          cell_add_neighbor(aux1,it3,world->cell_matrix[pos_aux.x][pos_aux.y]);

          for(it5 = 0; it5 < 5; it5++){
            if(( cell_exists((aux = cell_get_neighbor(aux1, neighbor_neigh[it5])) ) ) && cell_get_state(aux) == alive ){
			        v = cell_get_relative_to_neighbor(cell_get_relative_by_index(it3), cell_get_relative_by_index(neighbor_neigh[it5]));
			        index = cell_get_index_by_relative(v);
              cell_add_neighbor(aux, index ,  world->cell_matrix[pos_aux.x][pos_aux.y]);
            }
          }
#ifdef DEBUG

          cell_will_spawn_alert(  world->cell_matrix[pos_aux.x][pos_aux.y],aux1);
#endif
        }
      }
    }

  }

}

void world_update_gen(){
  int it1, it2;
  for(it1 = 0; it1 < world->size; it1 ++){
    for(it2 = 0; it2 < world->size; it2 ++){
      if( world->cell_matrix[it1][it2] != NULL)
        world->cell_matrix[it1][it2] = cell_list_update_state(world->cell_matrix[it1][it2]);
    }
  }
}

void world_order(){
  int it1,it2;
  for(it1 = 0;it1 < world->size;it1++){
    for(it2 = 0 ; it2 < world->size; it2++){
      if(world->cell_matrix[it1][it2] != NULL)
		  world->cell_matrix[it1][it2] = cell_order(world->cell_matrix[it1][it2]);
    }
  }
}


void world_print(FILE * file){
  int it1,it2;
  for(it1 = 0;it1 < world->size;it1++){
    for(it2 = 0 ; it2 < world->size; it2++){
      if(world->cell_matrix[it1][it2] != NULL) cell_list_print(world->cell_matrix[it1][it2], file);
    }
  }
}


void world_init( int size){
  int it;
  world = &world_st;
  world->size = size;
  world->cell_matrix = (cell_ptr **) malloc(sizeof(cell_ptr*) * size);
  for(it =  0; it < size; it++){
    world->cell_matrix[it] = (cell_ptr *) malloc(sizeof(cell_ptr) * size);
    memset(world->cell_matrix[it],0,sizeof(cell_ptr) * size);
  }

  world->destroy = world_destroy;
  world->map = world_map;
  world->add_cell = world_add_cell;
  world->get_next_gen =  world_get_next_gen;
  world->update_gen = world_update_gen;
  world->order =  world_order;
  world->print = world_print;

}
