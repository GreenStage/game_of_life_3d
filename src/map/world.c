#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>
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

cell_ptr world_get_next_list(int x, int y,int it, pos_ * retval){
  retval->x = x;
  retval->y = y;

  switch(it){
    case 0 :
      break;
    case 1 :
      retval->y = get_coord(y + 1,world->sizeZ);
      break;
    case 2 :
      retval->y = get_coord(y - 1,world->sizeZ);
      break;
    case 3 :
      retval->x = get_coord(x + 1,world->sizeZ);
      break;
    case 4 :
      retval->x = get_coord(x - 1,world->sizeZ);
      break;
    case 5 :
      retval->x = get_coord(x + 2,world->sizeZ);
      break;
    case 6 :
      retval->x = get_coord(x -2,world->sizeZ);
      break;
    case 7 :
      retval->y = get_coord(y + 2,world->sizeZ);
      break;
    case 8 :
      retval->y = get_coord(y -2,world->sizeZ);
      break;
    case 9 :
      retval->x = get_coord(x + 1,world->sizeZ);
      retval->y = get_coord(y + 1,world->sizeZ);
      break;
    case 10 :
      retval->x = get_coord(x + 1,world->sizeZ);
      retval->y = get_coord(y - 1,world->sizeZ);
      break;
    case 11 :
      retval->x = get_coord(x - 1,world->sizeZ);
      retval->y = get_coord(y + 1,world->sizeZ);
      break;
    case 12 :
      retval->x = get_coord(x - 1,world->sizeZ);
      retval->y = get_coord(y - 1,world->sizeZ);
      break;
    default:
      break;
  }
  if(world->pID == block_owner(world->sizeZ,*retval,world->dim_size,world->comm)){
    return world->cell_matrix[retval->x][retval->y];
  }
  else {
    return NULL;
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
  for(it =  0; it < world->sizeZ; it++){
    for(it2 = 0; it2 < world->sizeZ; it2++){
        cell_free_list(world->cell_matrix[it][it2]);
    }
  }
}

void world_add_cell(pos_ pos){
  world->cell_matrix[pos.x][pos.y] = insert_new_cell(world->cell_matrix[pos.x][pos.y],pos, world->sizeZ -1);

  cell_update_state(world->cell_matrix[pos.x][pos.y]);

}

void world_map(){
  MPI_Request Z_RQST, Z_RPLY, FINISHED;
  int it1, it2, it3;
  int z_rqst_flag = 0, z_rply_flag = 0, finished_flag = 0, logic_block = 0, recv_flag = 0;
  int peer, bufZsize, *bufZ;
  pos_ auxXY1, auxXY2;
  int first_neighbors_ctr, index;
  cell_ptr  aux1, aux2, aux3;
  mirror border;
#ifdef DEBUG
  int counter = 0;
#endif
  memset(world->zArray,-1,sizeof(int) * world->sizeZ);

  MPI_Irecv(&recv_flag, 1, MPI_INT, MPI_ANY_SOURCE, TAG_ALERT_FINISHED_MAP,world->comm, &FINISHED);
  MPI_Irecv(&auxXY1, sizeof(auxXY1), MPI_BYTE, MPI_ANY_SOURCE, TAG_RQST_Z_LST, world->comm, &Z_RQST);

  for(it1= 0, it2 = 0, aux1 = world->cell_matrix[0][0]; it1 < world->sizeZ;){

    while(!cell_exists(aux1)){
      if(it2 == world->sizeZ -1){
        it1 ++;
    it2 = 0;
        if(it1 == world->sizeZ) break;
      }
      else it2++;
      aux1 = world->cell_matrix[it1][it2];
    }
    if(it1 == world->sizeZ) break;

    first_neighbors_ctr = 0;

    MPI_Test(&FINISHED, &finished_flag, &world->status);
    if(finished_flag == 1){
      world->rqstArray[world->status.MPI_SOURCE] = recv_flag;
      if(!arrayFilled(world->rqstArray, world->pNumb)){
        MPI_Irecv(&recv_flag,1, MPI_INT, MPI_ANY_SOURCE, TAG_ALERT_FINISHED_MAP,world->comm, &FINISHED);
      } else
        break;
      finished_flag = 0;
    }

    MPI_Test(&Z_RQST, &z_rqst_flag, &world->status); //check for incoming requests
    if(z_rqst_flag == 1){
      bufZ = list_to_array(world->cell_matrix[auxXY1.x - world->smallWorldLimits[0]][auxXY1.y - world->smallWorldLimits[2]], &bufZsize, world->sizeZ);
      MPI_Send(bufZ, bufZsize, MPI_INT ,world->status.MPI_SOURCE, TAG_RPLY_Z_LST, world->comm);
      MPI_Irecv(&auxXY1, sizeof(auxXY1), MPI_BYTE, MPI_ANY_SOURCE, TAG_RQST_Z_LST, world->comm, &Z_RQST);
    }

    if(logic_block != -1){ //need answer
        MPI_Test(&Z_RPLY, &z_rply_flag, &world->status); //check for incoming requests
        if(z_rply_flag == 1){
          aux2 = arrayToList(auxXY2.x, auxXY2.y, world->zArray, world->sizeZ);
        }
    }



#ifdef DEBUG
      printf("Cell list processing at position %d\n", counter);
      counter++;
#endif

    border = near_none;

    if(logic_block != -1 && z_rply_flag){
      it3 = logic_block;
      logic_block = -1;
    }
    else if(logic_block != -1 && !z_rply_flag) {
      continue;
    }
    else if (logic_block == -1){
      it3 = 0;
      aux2 = world->cell_matrix[it1][it2];
    }

    for(; it3 < 13 ; it3++ , aux2 = world_get_next_list(it1, it2, it3,&auxXY2)){

      if(aux2 == NULL){ /* ASK OtHEr pROCESS */
        peer = block_owner(world->sizeZ, auxXY2,world->dim_size,world->comm) ;
        MPI_Send(&auxXY2,sizeof(auxXY2),MPI_BYTE,peer,TAG_RQST_Z_LST,world->comm);
        MPI_Irecv(world->zArray, world->sizeZ, MPI_INT, peer, TAG_RPLY_Z_LST, world->comm, &Z_RPLY);
        logic_block = it3;
        z_rply_flag = 0;
        break;
      }

      for(aux3 = aux2; aux3 != NULL; aux3 = cell_get_next(aux3)){
        border = cell_get_near_border(aux3);

        if (-1 != (index = cell_get_diamond_index(aux3, aux1, get_pos_by_it(it3), world->sizeZ,near_none)))
          cell_set_neighbors(aux1, aux3, index);

        if (-1 != ( index = cell_get_diamond_index(aux3, aux1, get_pos_by_it(it3), world->sizeZ, border & (near_zmax | near_z0))) )
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
  for(it1= 0, it2 = 0, aux1 = world->cell_matrix[0][0]; it1 < world->sizeZ; aux1 = cell_get_next(aux1)){

	while (!cell_exists(aux1)) {
		if (it2 == world->sizeZ - 1) {
			it1++;
			it2 = 0;
			if (it1 == world->sizeZ) break;
		}
		else it2++;
		aux1 = world->cell_matrix[it1][it2];
	}
	if (it1 == world->sizeZ) break;
	if (cell_get_state(aux1) != alive) continue;

    for(neighbors_count = 0, it3 = 0; it3 < 6; it3 ++){
		    aux2 = cell_get_neighbor(aux1, it3);
		    if( cell_exists( aux2 ) && cell_get_state( aux2 ) == alive )
          neighbors_count ++;
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
          pos_aux = cell_get_absolute_pos(aux1,it3,world->sizeZ - 1);
          world->cell_matrix[pos_aux.x][pos_aux.y] = insert_new_cell(world->cell_matrix[pos_aux.x][pos_aux.y],pos_aux,world->sizeZ - 1) ;
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
  for(it1 = 0; it1 < world->sizeZ; it1 ++){
    for(it2 = 0; it2 < world->sizeZ; it2 ++){
      if( world->cell_matrix[it1][it2] != NULL)
        world->cell_matrix[it1][it2] = cell_list_update_state(world->cell_matrix[it1][it2]);
    }
  }
}

void world_order(){
  int it1,it2;
  for(it1 = 0;it1 < world->sizeZ;it1++){
    for(it2 = 0 ; it2 < world->sizeZ; it2++){
      if(world->cell_matrix[it1][it2] != NULL)
		  world->cell_matrix[it1][it2] = cell_order(world->cell_matrix[it1][it2]);
    }
  }
}


void world_print(FILE * file){
  int it1,it2;
  for(it1 = 0;it1 < world->sizeZ;it1++){
    for(it2 = 0 ; it2 < world->sizeZ; it2++){
      if(world->cell_matrix[it1][it2] != NULL) cell_list_print(world->cell_matrix[it1][it2], file);
    }
  }
}

int fetch_borders(int ){
  int it = 0, it2 = 0, it3;
  cell_ptr cheater;
  int * sendBufZ = NULL;
  int * recvBufZ = (int *) malloc(sizeof(int)*(world->sizeZ + 2));
  int myCords[2];
  int sendXY[2];
  int rank, x, y;
  int recvCtr = 0;
  int z_receive_flag = 0, recvBufSize, recvIndex;
  MPI_Request req;
  int toSendSize;
  int toSendX[4], toSendY[4];
  int peerCords[4][2];
  int peerRank[4];
  int breakCounter = 0;


  MPI_Cart_coords(comm,world->pID,2,cords);

  peerCords[0][0] = myCords[0];
  peerCords[0][1] = get_coord(cords[1] - 1,worlds->dim_size[1]);
  MPI_Cart_rank(world->comm, peerCords[0], &peerRank[0]);
  toSendX[0] = 0;
  toSendY[0] = 0;

  peerCords[1][0] = myCords[0];
  peerCords[1][1] = get_coord(cords[1] + 1,worlds->dim_size[1]);
  MPI_Cart_rank(world->comm, peerCords[1], &peerRank[1]);
  toSendX[1] = 0;
  toSendY[1] = world->sizeY - 1;

  peerCords[2][0] = get_coord(cords[0] - 1,worlds->dim_size[0]);
  peerCords[2][1] = myCords[1];
  MPI_Cart_rank(world->comm, peerCords[2], &peerRank[2]);
  toSendX[2] = 0;
  toSendY[2] = 0;

  peerCords[3][0] = get_coord(cords[0] + 1,worlds->dim_size[0]);
  peerCords[3][1] = myCords[1];
  MPI_Cart_rank(world->comm, peerCords[3], &peerRank[3]);
  toSendX[3] = world->sizeX - 1;
  toSendY[3] = 0;

  MPI_Irecv(recvBufZ, world->sizeZ + 2, MPI_INT, MPI_ANY_SOURCE, TAG_RPLY_Z_LST, world->comm, &Z_RPLY);
  for(;;){

    MPI_Test(&Z_RPLY, &z_receive_flag, &world->status); //check for the reception of any array
    if(z_array_flag){

      MPI_Get_count(world->status, MPI_INT, &recvBufSize);
      if(recvBufSize == 1){ //received message
        if(++breakCounter == 4)
          break;
      }

      for(it3 = 0; it3 < 4; it3++){ //it3 corresponds to a relative position of a processor: 0 up; 1 down; 2 left;3 right
        if(world->status.MPI_SOURCE == peerRank[it3]){
          //translate 2 dimensional coords to vector index
         recvIndex = coordsToArray(it3, recvBufZ[0], recvBufZ[1]);
         world->border[it3].cells[recvIndex] = arrayToList(recvBufZ[0],recvBufZ[1],recvBufZ, recvBufSize, world->sizeZ, 2); //insert new cell uses recvBufSize as maxPos.. Problem??
        }
      }
      MPI_Irecv(recvBufZ, world->sizeZ + 2, MPI_INT, MPI_ANY_SOURCE, TAG_RPLY_Z_LST, world->comm, &Z_RPLY);
    }


    if( it < 4 ){

      if(it2 < (it < 2)? world->sizeX : world->sizeY){
        if(it < 2){
          toSendX[it]++; //iterates through the line at toSendY[it]
        } else{
          toSendY[it]++; //iterates through the column at toSendX[it]
        }

        toSendSize = 2;
        sendBufZ = list_to_array(world->cell_matrix[toSendX[it]][toSendY[it]], &toSendSize, world->sizeZ + 2, 2);

        if(sendBufZ != NULL){
          sendBufZ[0] = toSendX[it];
          sendBufZ[1] = toSendY[it];
          MPI_Isend(sendBufZ, toSendSize, MPI_INT, peerRank[it], TAG_RPLY_Z_LST, world->comm, &req);
        }

        it2++;
      } else{
        it2 = 0;
        MPI_Isend(&it2, 1, MPI_INT, peerRank[it], TAG_RPLY_Z_LST, world->comm, &req); //send break signal; all cells sent to peerRank[it]
        it++;
      }
  }

}

void world_init( int sizeX, int sizeY, int sizeZ, int smallWorldLimits[4], int pid, MPI_Comm comm,int dim_size[2],int pNumb){
  int it;
  world = &world_st;
  world->sizeX = sizeX;
  world->sizeY = sizeY;
  world->sizeZ = sizeZ;
  memcpy(world->smallWorldLimits,smallWorldLimits, 4*sizeof(int));
  world->zArray = (int *) calloc(world->sizeZ,sizeof(int));
  world->pID = pid;
  world->comm = comm;
  world->pNumb = pNumb;
  world->rqstArray = (int *) calloc(world->pNumb, sizeof(int) );
  memcpy(world->dim_size,dim_size,2*sizeof(int));

  world->cell_matrix = (cell_ptr **) malloc(sizeof(cell_ptr*) * sizeX);
  for(it =  0; it < sizeX; it++){
    world->cell_matrix[it] = (cell_ptr *) malloc(sizeof(cell_ptr) * sizeY);
    memset(world->cell_matrix[it],0,sizeof(cell_ptr) * sizeY);
  }

  for(it = 0; it < 4; it++){
    world->borders[it] = (cell_ptr *) malloc(sizeof(cell_ptr) * (it < 2)? sizeX : sizeY );
    memset(world->borders[it], 0, sizeof(cell_ptr) * (it < 2)? sizeX : sizeY );
  }

  world->destroy = world_destroy;
  world->map = world_map;
  world->add_cell = world_add_cell;
  world->get_next_gen =  world_get_next_gen;
  world->update_gen = world_update_gen;
  world->order =  world_order;
  world->print = world_print;

}
