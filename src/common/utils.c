#include "defs.h"
#include <stdlib.h>
#include <stdio.h>
int block_owner(int cube_size,pos_ pos,int * dim_size,MPI_Comm comm){
  int i[2];
  int retval = -1;
  int normSizeX, normSizeY;

  normSizeX = cube_size / dim_size[0];
  normSizeY = cube_size / dim_size[1];

  for(i[0] = 0; i[0] < dim_size[0]; i[0]++){
    if( (i[0] + 1) * normSizeX > pos.x){
      break;
    }
  }

  for(i[1] = 0; i[1] < dim_size[1]; i[1]++){
    if( (i[1] + 1) * normSizeY > pos.y){
      break;
    }
  }

  return (MPI_ERR_TOPOLOGY == MPI_Cart_rank(comm,i,&retval) ) ? -1 : retval;
}

int get_sizes_by_limits(int blockLimits[4], int retval[2]){
  retval[0] = blockLimits[1] - blockLimits[0];
  retval[1] = blockLimits[3] - blockLimits[2];
  return 0;
}

int get_block_limits(int block_owner,int cube_size,int * dim_size, int retval[4], MPI_Comm comm){
  int dim_cords[2];
   int normSizeX, normSizeY;

  MPI_Cart_coords(comm,block_owner,2,dim_cords);

  normSizeX = cube_size / dim_size[0];
  normSizeY = cube_size / dim_size[1];

  retval[0] = normSizeX * dim_cords[0];
  retval[1] = (dim_cords[0] == dim_size[0] - 1) ? cube_size : normSizeX * (dim_cords[0] + 1) ;

  retval[2] = normSizeY * dim_cords[1];
  retval[3] = (dim_cords[1] == dim_size[1] - 1) ? cube_size : normSizeY * (dim_cords[1] + 1) ;

  return 0;
}

int arrayFilled(int * zArray,int pNumb){
  int i;
  for(i = 0; i < pNumb - 1; i++){
    if(zArray[i] != 1){
      return 0;
    }
  }
  return 1;
}

int coordsToArray(int rank, int recvCoords[2], int ownCoords[2]){
  int index;

  if(peerCoords[0] == ownCoords[0]){

  }
  return index;
}
