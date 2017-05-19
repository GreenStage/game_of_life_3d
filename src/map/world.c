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

void desnormalize(int * x, int * y){
  *x += world->smallWorldLimits[0];
  *y += world->smallWorldLimits[2];
}

void normalize(int * x , int * y){
  *x -= world->smallWorldLimits[0];
  *y -= world->smallWorldLimits[2];
}

cell_ptr world_get_next_list(int x, int y,int it){
  int a = x;
  int b = y;
  desnormalize(&x,&y);

  switch(it){
    case 0 :
      break;
    case 1 :
      b++;
      y = get_coord(y + 1,world->sizeZ);
      break;
    case 2 :
      b--;
      y = get_coord(y - 1,world->sizeZ);
      break;
    case 3 :
      a++;
      x = get_coord(x + 1,world->sizeZ);
      break;
    case 4 :
      a--;
      x = get_coord(x - 1,world->sizeZ);
      break;

    case 5 :
      a += 2;
      x = get_coord(x + 2,world->sizeZ);
      break;
    case 6 :
      a -= 2;
      x = get_coord(x -2,world->sizeZ);
      break;
    case 7 :
      b += 2;
      y = get_coord(y + 2,world->sizeZ);
      break;
    case 8 :
      b -= 2;
      y = get_coord(y -2,world->sizeZ);
      break;
    case 9 :
      a++;
      b++;
      x = get_coord(x + 1,world->sizeZ);
      y = get_coord(y + 1,world->sizeZ);
      break;
    case 10 :
      a++;
      b--;
      x = get_coord(x + 1,world->sizeZ);
      y = get_coord(y - 1,world->sizeZ);
      break;
    case 11 :
      a--;
      b++;
      x = get_coord(x - 1,world->sizeZ);
      y = get_coord(y + 1,world->sizeZ);
      break;
    case 12 :
      a--;
      b--;
      x = get_coord(x - 1,world->sizeZ);
      y = get_coord(y - 1,world->sizeZ);
      break;
    default:
      break;
  }
  normalize(&x,&y);



  if(x < world->sizeX && y < world->sizeY && y >=0 && x >= 0){
    return world->cell_matrix[x][y];
  }

 /* else if(a >= 0 && a < world->sizeX && b >= 0 & b < world->sizeY){
    return world->cell_matrix[a][b];
  }*/

  else if(a == -1 && b >=  0 && b < world->sizeY){
    return world->borders[2].cells[b];
  }

  else if(a == world->sizeX && b >=  0 && b < world->sizeY){
    return world->borders[3].cells[b];
  }
  else if(b == -1 && a >=  0 && a < world->sizeX){
    return world->borders[0].cells[a];
  }
  else if(b == world->sizeY && a >=  0 && a < world->sizeX){
    return world->borders[1].cells[a];
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
  for(it =  0; it < world->sizeX; it++){
    for(it2 = 0; it2 < world->sizeY; it2++){
        cell_free_list(world->cell_matrix[it][it2]);
    }
  }
}

void world_add_cell(pos_ pos){
  world->cell_matrix[pos.x][pos.y] = insert_new_cell(world->cell_matrix[pos.x][pos.y],pos, world->sizeZ -1);

  cell_update_state(world->cell_matrix[pos.x][pos.y]);

}

void get_common_corner(int it,int * retval, int *b1Index, int *c1Index,  int *b2Index,int *c2Index){
  switch(it){
    case 0:
      retval[0] =0;
      retval[1] =0;
      *b1Index = 0;
      *c1Index = 0;
      *b2Index = 2;
      *c2Index = 0;
      break;
    case 1:
      retval[0] =world->sizeX -1;
      retval[1] =0;
      *b1Index = 0;
      *c1Index = 1;
      *b2Index = 3;
      *c2Index = 0;   
      break;
    case 2:
      retval[0] = 0;
      retval[1] = world->sizeY -1;
      *b1Index = 1;
      *c1Index = 0;
      *b2Index = 2;
      *c2Index = 1;   
      break;
    default:
      retval[0] = world->sizeX -1;
      retval[1] = world->sizeY -1;
      *b1Index = 1;
      *c1Index = 1;
      *b2Index = 3;
      *c2Index = 1;   
      break;
  }
}

void world_map(){
  int it1, it2, it3;
  int first_neighbors_ctr, index;
  cell_ptr  aux1, aux2, aux3;
  mirror border;
#ifdef DEBUG
  int counter = 0;
#endif
/*aux1 = world->cell_matrix[0][0];
  while( world->pID == 0 &&  aux1!= NULL ){
    printf("aux1: %d %d %d\n",cell_get_pos(aux1).x,cell_get_pos(aux1).y,cell_get_pos(aux1).z);
    aux1 = cell_get_next(aux1);
  }*/
  for(it1= 0, it2 = 0, aux1 = world->cell_matrix[0][0]; it1 < world->sizeX;){

    while(!cell_exists(aux1)){
      if(it2 == world->sizeY -1 ){
        it1 ++;
	    	it2 = 0;
        if(it1 == world->sizeX) break;
      }
      else it2++;
      aux1 = world->cell_matrix[it1][it2];
    }
    if(it1 == world->sizeX) break;

    first_neighbors_ctr = 0;

#ifdef DEBUG
      printf("Cell list processing at position %d\n", counter);
      counter++;
#endif
    border = near_none;

    for(it3 = 0, aux2 = world->cell_matrix[it1][it2]; it3 < 13 ; it3++ , aux2 = world_get_next_list(it1, it2, it3)){
      /*if(aux2 != NULL && world->pID == 3)
        printf("MAP: pID %D -- IT1 = %d , IT2 = %d  %d %d %d fetching neighbor list at position %d %d\n\n",world->pID,it1,it2,cell_get_pos(aux1).x,cell_get_pos(aux1).y,cell_get_pos(aux1).z,cell_get_pos(aux2).x,cell_get_pos(aux2).y);
      */
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
void list_to_bool(int *arr,cell_ptr head){
  cell_ptr aux;
  for(aux = head; cell_exists(aux);aux = cell_get_next(aux)){
    arr[cell_get_pos(aux).z] = 1;
  }
}

void world_get_next_gen(){
  int b1,b2;
  int * auxCorner;
  int it1,it2;
  int it3, it4, it5;
  int itc,notAdd; 
  int ita,itb;
  int c1,c2;
  int index,v;
  int xNorm, yNorm;
  int neighbors_count;
  int neighbor_neigh[5];
  int cornersA[2];
  cell_ptr aux,aux1,aux2,aux5;
  mirror border = near_none;
  pos_ pos_aux;
  auxCorner = (int*) calloc(world->sizeZ,sizeof(int));
  for(it1= 0, it2 = 0, aux1 = world->cell_matrix[0][0]; it1 < world->sizeX; aux1 = cell_get_next(aux1)){

	while (!cell_exists(aux1)) {

		if (it2 == world->sizeY - 1) {
			it1++;
			it2 = 0;

			if(it1 == world->sizeX) {
        break;
      }
		}

		else it2++;
		aux1 = world->cell_matrix[it1][it2];
	}

	if (it1 == world->sizeX ) break;

	if (cell_get_state(aux1) != alive) continue;

    for(neighbors_count = 0, it3 = 0; it3 < 6; it3 ++){
		    aux2 = cell_get_neighbor(aux1, it3);
		    if( cell_exists( aux2 ) && cell_get_state( aux2 ) == alive )
          neighbors_count ++;
    }
    
    cell_find_next_state(aux1,neighbors_count);

    for( it3=0 ; it3 < 6 ; it3++ ){ /*Dead neighbor should be born?*/
      
      /*Check if neighbor belongs to process sub-world*/
      pos_aux = cell_get_absolute_pos(aux1,it3,world->sizeZ - 1);
      //TODO PICO DE DAR MErDA
      if(world->pID != block_owner(world->sizeZ,pos_aux,world->dim_size,world->comm)){
        continue;
      }

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
          xNorm = pos_aux.x;
          yNorm = pos_aux.y;
          normalize(&xNorm,&yNorm);
          world->cell_matrix[xNorm][yNorm] = insert_new_cell(world->cell_matrix[xNorm][yNorm],pos_aux,world->sizeZ - 1) ;
          cell_add_neighbor(aux1,it3,world->cell_matrix[xNorm][yNorm]);

          for(it5 = 0; it5 < 5; it5++){
            if(( cell_exists((aux = cell_get_neighbor(aux1, neighbor_neigh[it5])) ) ) && cell_get_state(aux) == alive ){
			        v = cell_get_relative_to_neighbor(cell_get_relative_by_index(it3), cell_get_relative_by_index(neighbor_neigh[it5]));
			        index = cell_get_index_by_relative(v);
              cell_add_neighbor(aux, index ,  world->cell_matrix[xNorm][yNorm]);
            }
          }
#ifdef DEBUG
          cell_will_spawn_alert(  world->cell_matrix[pos_aux.x][pos_aux.y],aux1);
#endif
        }
      }
    }

  }
  for(ita = 0; ita < 4;ita ++){

     memset(auxCorner,0,sizeof(int) * world->sizeZ);
     get_common_corner(ita,cornersA,&b1,&c1,&b2,&c2);
     //printf("Get commoncorder returned %d %d with corners %d->%d and %d->%d\n",cornersA[0],cornersA[1],b1,c1,b2,c2);
     list_to_bool(auxCorner,world->cell_matrix[cornersA[0]][cornersA[1]]);
     if(cornersA[0] != -1 && cornersA[1] != -1){
       
        for(itc = 0; itc < world->sizeZ;itc ++){
          printf("corner is %d, bc1 is %d, bc2 is %d\n",auxCorner[itc], world->borders[b1].corners[c1].zArray[itc],world->borders[b2].corners[c2].zArray[itc]);
          if(auxCorner[itc] == 0 && world->borders[b1].corners[c1].zArray[itc] == 1 && world->borders[b2].corners[c2].zArray[itc] == 1){
              pos_aux.x = cornersA[0];
              pos_aux.y = cornersA[1];
              pos_aux.z = itc;
              desnormalize(&pos_aux.x,&pos_aux.y);
              printf("Adding to %d %d \n",cornersA[0],cornersA[1]);
              world->cell_matrix[cornersA[0]][cornersA[1]] = insert_new_cell(world->cell_matrix[cornersA[0]][cornersA[1]],pos_aux,world->sizeZ - 1) ;
            
          }

          
        }
     }
     
  }
  free(auxCorner);
}

void world_update_gen(){
  int it1, it2;
  for(it1 = 0; it1 < world->sizeX; it1 ++){
    for(it2 = 0; it2 < world->sizeY; it2 ++){
      if( world->cell_matrix[it1][it2] != NULL)
        world->cell_matrix[it1][it2] = cell_list_update_state(world->cell_matrix[it1][it2]);
    }
  }
}

void world_order(){
  int it1,it2;
  for(it1 = 0;it1 < world->sizeX;it1++){
    for(it2 = 0 ; it2 < world->sizeY; it2++){
      if(world->cell_matrix[it1][it2] != NULL){
		   world->cell_matrix[it1][it2] = cell_order(world->cell_matrix[it1][it2]);
      }

    }
  }
}


void world_print(FILE * file){
  int it1,it2;
  for(it1 = 0;it1 < world->sizeX;it1++){
    for(it2 = 0 ; it2 < world->sizeY; it2++){
      if(world->cell_matrix[it1][it2] != NULL) cell_list_print(world->cell_matrix[it1][it2], file);
    }
  }
}

void fetch_borders(){
  int it = 0, it2 = 0, it3, it5 = 0, it6 = 0;
  cell_ptr aux,aux2;
  int * sendBufZ = NULL;
  int * recvBufZ = (int *) calloc(world->sizeZ + 2,sizeof(int));
  int myCords[2];
  int sendXY[2];
  int rank, x, y;
  int recvCtr = 0;
  int z_receive_flag = 0, recvBufSize, recvIndex;
  MPI_Request req,Z_RPLY;
  int toSendSize;
  int toSendX[4], toSendY[4];
  int peerCords[4][2];
  int peerRank[4];
  int breakCounter = 0;
  int relativeBorder[4];

  cell_ptr aux4;
  world->test = 0;
  world->test2 = 0;
 
  /*if(world->pID == 2){
      printf("2 sizeX: %d  sizeY: %d  sizeZ: %d\n", world->sizeX, world->sizeY, world->sizeZ);
  }*/
  for(it = 0; it < 4; it++){
  

    for(it2 = 0; (it2 < world->sizeX && it < 2)  || (it2 < world->sizeY && it >= 2); it2++){
       cell_free_list(world->borders[it].cells[it2]);
       world->borders[it].cells[it2] = NULL;   
    }

  }
  it = it2 = 0;

  MPI_Cart_coords(world->comm,world->pID,2,myCords);

  peerCords[0][0] = myCords[0];
  peerCords[0][1] = get_coord(myCords[1] - 1,world->dim_size[1]); //vai dar a y menores
  relativeBorder[0] = 1;
  MPI_Cart_rank(world->comm, peerCords[0], &peerRank[0]);
  toSendX[0] = 0;
  toSendY[0] = 0;
 
  peerCords[1][0] = myCords[0];
  peerCords[1][1] = get_coord(myCords[1] + 1,world->dim_size[1]); 
  relativeBorder[1] = 0;
  MPI_Cart_rank(world->comm, peerCords[1], &peerRank[1]);
  toSendX[1] = 0;
  toSendY[1] = world->sizeY - 1;

  peerCords[2][0] = get_coord(myCords[0] - 1,world->dim_size[0]);
  peerCords[2][1] = myCords[1];
  relativeBorder[2] = 3;
  MPI_Cart_rank(world->comm, peerCords[2], &peerRank[2]);
  toSendX[2] = 0;
  toSendY[2] = 0;

  peerCords[3][0] = get_coord(myCords[0] + 1,world->dim_size[0]);
  peerCords[3][1] = myCords[1];
  relativeBorder[3] = 2;
  MPI_Cart_rank(world->comm, peerCords[3], &peerRank[3]);
  toSendX[3] = world->sizeX - 1;
  toSendY[3] = 0;

 //printf("Process %d coords %d %d: UP %d DOWN %d LEFT %d RIGHT %d\n", world->pID, myCords[0], myCords[1], peerRank[0], peerRank[1], peerRank[2], peerRank[3]);

  //printf("Process %d coords %d %d: UP %d DOWN %d LEFT %d RIGHT %d\n", world->pID, myCords[0], myCords[1], peerRank[0], peerRank[1], peerRank[2], peerRank[3]);
  MPI_Irecv(recvBufZ, world->sizeZ + 2, MPI_INT, MPI_ANY_SOURCE, TAG_RPLY_Z_LST, world->comm, &Z_RPLY);
  for(;;){
    
    if(it >= 4 && breakCounter == 4){ //break condition
      break;
    }

    MPI_Test(&Z_RPLY, &z_receive_flag, &world->status); //check for the reception of any array
    if(z_receive_flag){

      MPI_Get_count(&world->status, MPI_INT, &recvBufSize);
      if(recvBufSize == 1){ //received message
      //printf("process %d received break from %d\n", world->pID, world->status.MPI_SOURCE);
        MPI_Irecv(recvBufZ, world->sizeZ + 2, MPI_INT, MPI_ANY_SOURCE, TAG_RPLY_Z_LST, world->comm, &Z_RPLY);
        breakCounter++;
      } else{
        //normalize(&recvBufZ[0],&recvBufZ[1]);
        for(it3 = 0; it3 < 4; it3++){ //it3 corresponds to a relative position of a processor: 0 up; 1 down; 2 left;3 right
          if(world->status.MPI_SOURCE == peerRank[it3]){
            //translate 2 dimensional coords to vector index
          

            /*  for(it5 = 2; it5 < recvBufSize; it5 ++){
                printf("[%d] received from %d   x y z %d %d %d\n", world->pID, world->status.MPI_SOURCE,recvBufZ[0], recvBufZ[1], recvBufZ[it5]);
              }
              */
             //if(recvBufZ[0] > 24 || recvBufZ[1] > 24 || recvBufZ[it5] > 50)
             // printf("p%d normalized z_bordered: (%d,%d,%d)\n",world->pID, recvBufZ[0],recvBufZ[1],recvBufZ[it5]);
            normalize(&recvBufZ[0],&recvBufZ[1]);
            recvIndex = coordsToArray(it3, recvBufZ[0], recvBufZ[1], world->smallWorldLimits[0], world->smallWorldLimits[2]);
            desnormalize(&recvBufZ[0],&recvBufZ[1]);

            switch(recvBufZ[2]){
              case 0:
                world->cell_matrix[recvIndex +1][0] = arrayToList(recvBufZ[0],recvBufZ[1],recvBufZ, recvBufSize, world->sizeZ, 3); //insert new cell uses recvBufSize as maxPos.. Problem??} 
                break;
              case 1:
                world->cell_matrix[recvIndex +1][world->sizeX -1] = arrayToList(recvBufZ[0],recvBufZ[1],recvBufZ, recvBufSize, world->sizeZ, 3); //insert new cell uses recvBufSize as maxPos.. Problem??} 
                break; 
              case 2:
                world->cell_matrix[0][recvIndex] = arrayToList(recvBufZ[0],recvBufZ[1],recvBufZ, recvBufSize, world->sizeZ, 3); //insert new cell uses recvBufSize as maxPos.. Problem??} 
                break;   
              case           
            }
            
        } 
        MPI_Irecv(recvBufZ, world->sizeZ + 2, MPI_INT, MPI_ANY_SOURCE, TAG_RPLY_Z_LST, world->comm, &Z_RPLY);
      }
    }


    if( it < 4 ){

      if((it2 < world->sizeX && it < 2)  || (it2 < world->sizeY && it >= 2)){
        
        //printf("p%d sending matrixPos x: %d y: %d (it2 %d) to p%d\n", world->pID, toSendX[it], toSendY[it], it2, peerRank[it]);
        sendBufZ = list_to_array(world->cell_matrix[toSendX[it]][toSendY[it]], &toSendSize, world->sizeZ + 2, 3);
       // for(it3 = 2; it3 < toSendSize; it3 ++){
          //printf("p%d z_bordered: (%d,%d,%d)\n",world->pID, toSendX[it],toSendY[it] ,sendBufZ[it3]);
        //}
        
        if(sendBufZ != NULL){
          sendBufZ[0] = toSendX[it];
          sendBufZ[1] = toSendY[it];
          sendBufZ[2] = relativeBorder[it];
          desnormalize(&sendBufZ[0],&sendBufZ[1]);
  
           /*for(it5 = 2; it5 < toSendSize; it5 ++){
              printf("[%d] Sending to %d   x y z %d %d %d\n", world->pID, peerRank[it],sendBufZ[0], sendBufZ[1], sendBufZ[it5]);
            }*/
          
          MPI_Send(sendBufZ, toSendSize, MPI_INT, peerRank[it], TAG_RPLY_Z_LST, world->comm);
          free(sendBufZ);
        }
        //printf(" it2 %d  to %d\n", it2, toSendX[it]);
        if(it < 2){
          toSendX[it]++; //iterates through the line at toSendY[it]
        } else{
          toSendY[it]++; //iterates through the column at toSendX[it]
        }
        
        it2++;
        
      } else{
        //printf("ENTIRE BORDER SENT p%d\n", world->pID);
        it2 = 0;
        MPI_Isend(&it2, 1, MPI_INT, peerRank[it], TAG_RPLY_Z_LST, world->comm, &req); //send break signal; all cells sent to peerRank[it]
        it++;
      }
    }
  }
 /* if(world->pID != 1) return;
  for(it2 = 0; it2 < 4; it2++){
    for(it = 0; it < 2 ; it++){
      printf(" Border %d corner %d:\n",it2,it);
      for(it3 = 0; it3< world->sizeZ;it3++){
        if(world->borders[it2].corners[it].zArray[it3] == 1)printf("%d has 1\n",it3);
      }
    }
  }*/
}
void world_print_line(int lineIndex){
  int i;
  for(i = 0; i < world->sizeY; i++){
    cell_list_print(world->cell_matrix[lineIndex][i],stdout);
  }
}
void world_init( int sizeX, int sizeY, int sizeZ, int smallWorldLimits[4], int pid, MPI_Comm comm,int dim_size[2],int pNumb){
  int it,it2;
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

  for(it = 0; it < 2; it++){
    world->borders[it].cells = (cell_ptr *) malloc(sizeof(cell_ptr) * sizeX );
    memset(world->borders[it].cells, 0, sizeof(cell_ptr) * sizeX );
    for(it2 = 0; it2<2; it2++){
      world->borders[it].corners[it2].zArray = (int*) calloc(world->sizeZ,sizeof(int));
    }
  }

  for(it = 2; it < 4; it++){
    world->borders[it].cells = (cell_ptr *) malloc(sizeof(cell_ptr) * sizeY);
    memset(world->borders[it].cells, 0, sizeof(cell_ptr) * sizeY );

    for(it2 = 0; it2<2; it2++){
      world->borders[it].corners[it2].zArray = (int*) calloc(world->sizeZ,sizeof(int));
    }
  }

  world->destroy = world_destroy;
  world->map = world_map;
  world->add_cell = world_add_cell;
  world->get_next_gen =  world_get_next_gen;
  world->update_gen = world_update_gen;
  world->order =  world_order;
  world->print = world_print;
  world->fetch_borders = fetch_borders;
  world->print_line = world_print_line;
}
