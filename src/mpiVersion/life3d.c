#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "../common/defs.h"
#include "../common/error.h"
#include "../map/world.h"
#include "../map/cell.h"

#ifdef DEBUG_TIME
#include <time.h>
#endif

int main(int argc, char * argv[]){

  int generations, cube_size = 0;
  pos_ pos;
  int a,i;
  int id, p, rank;
  int testflag, end;
  int partSizeX, partSizeY;
  int dims_size[2];
  int periodic[2] = {0,0};
  int blocklimits[4];
  int arraySize;
  int coco[2],xixi,bardaAe;
  int * bufZ;
  int oo, uu, xy[2];
  MPI_Comm cart_comm;
  MPI_Status status;
  MPI_Request request;

  cell_ptr ** cell_matrix;

  struct world_stct temp_world;
  pos_ * temp_pos;

  char line[MAX_LINE_SIZE];
  FILE * inputFile;
  char output_file[50];
#ifdef DEBUG_TIME
  clock_t start_t, end_t;
  double total_t;
  start_t = clock();
#endif

  if(argc < 3){
    error_exit("Error: Missing argument",ERR_MISSING_ARG);
  }

  if( strcmp( ".in" , ( argv[1] + strlen(argv[1]) - 3 ) ) != 0 ){
    error_exit("Error: Invalid input file format",ERR_INVALID_INPUT);
  }

  if( ( generations = atoi(argv[2]) ) == 0){
    error_exit("Error: Invalid generations number",ERR_INVALID_GEN);
  }

  world = NULL;


  /*Initialize MPI*/
  MPI_Init (&argc, &argv);
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Comm_rank (MPI_COMM_WORLD, &id); /*Get process rank*/
  MPI_Comm_size (MPI_COMM_WORLD, &p); /*Get number of processes*/

  /*Create Grid*/
  dims_size[0] = dims_size[1] = 0;
  MPI_Dims_create(p,2,dims_size);
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims_size, periodic, 0, &cart_comm);


  if( id == ROOT){


    inputFile = fopen(argv[1], "r");
    if(inputFile == NULL){
      error_exit("Error: Cant open %s for reading",ERR_OPEN_FILE);
    }
    fgets(line,MAX_LINE_SIZE,inputFile);
    if (!sscanf(line,"%d", &cube_size)){
      error_exit("Error: Invalid cube size",ERR_INVALID_SIZE);
    }
    printf("--------P MATRIX---------\n");
    for(oo = 0; oo < dims_size[0]; oo++){
      for(uu = 0; uu < dims_size[1]; uu++){
        int a[2] = {oo,uu};
        int rankp;
        int limits3[4];
        MPI_Cart_rank(cart_comm,a,&rankp);
        get_block_limits(rankp,cube_size,dims_size,limits3,cart_comm);
        printf("| %d  (%d, %d)(%d %d %d %d)| ",rankp, a[0], a[1],limits3[0],limits3[1],limits3[2],limits3[3]);
      }
      printf("\n-----------------------\n");
    }
    printf("\n");
    cell_matrix = (cell_ptr **) malloc(sizeof(cell_ptr*) * cube_size);
    for(i = 0; i < cube_size; i++){
        cell_matrix[i] = (cell_ptr *) malloc(sizeof(cell_ptr) * cube_size);
        memset(cell_matrix[i],0,sizeof(cell_ptr) * cube_size);
    }
  }

  MPI_Bcast(&cube_size,1,MPI_INT,ROOT,cart_comm);

  if( id == ROOT ){
    get_block_limits(ROOT,cube_size,dims_size,blocklimits, cart_comm);
    get_sizes_by_limits(blocklimits,coco);

    world_init(coco[0],coco[1],cube_size, blocklimits,id,cart_comm,dims_size,p);

    temp_pos = (pos_ *) malloc(sizeof(pos_) * cube_size);

    for(a=0; fgets(line,MAX_LINE_SIZE,inputFile) ;a++){

      if (!sscanf(line,"%d %d %d", &pos.x,&pos.y,&pos.z)){
        error_exit("Error: Invalid position",ERR_INVALID_POS);
      }
      /*printf("pos: %d %d %d \n",pos.x,pos.y,pos.z);
      printf("bo : %d\n",block_owner(cube_size,pos,dims_size,cart_comm));
      printf("%d\n",(int) &cell_matrix[pos.x][pos.y]);
      getchar();*/
      
      cell_matrix[pos.x][pos.y] = insert_new_cell(cell_matrix[pos.x][pos.y],pos, cube_size -1);
      cell_update_state(cell_matrix[pos.x][pos.y]);
      //printf("aiai %d %d\n",pos.x,pos.y);
      if(ROOT == block_owner(cube_size,pos,dims_size,cart_comm)){
        world->add_cell(pos);
      }

    }

    for(a = 1; a < p; a++){
      get_block_limits(a,cube_size,dims_size,blocklimits, cart_comm);
      MPI_Send(blocklimits, 4 , MPI_INT, a, TAG_ALERT_SIZE,cart_comm);
    }
    for(i = 0; i < cube_size; i++){
      for(a = 0; a < cube_size; a++){

        pos.x = i;
        pos.y = a;
        //printf("BLock owner : %d  %d\n",  block_owner(cube_size,pos,dims_size,cart_comm), a);
        bardaAe = 0;
        bufZ = list_to_array(cell_matrix[i][a], &bardaAe, world->sizeZ + 2, 0);
        if(bufZ == NULL){
          continue;
        }
        if(bardaAe > 0){
          pos.x = i;
          pos.y = a;

          xixi = block_owner(cube_size,pos,dims_size,cart_comm);

          coco[0] = i;
          coco[1] = a;

          if(xixi != ROOT){
            MPI_Send(coco, 2 , MPI_INT, xixi, TAG_ALERT_XY,cart_comm);
            MPI_Send(bufZ, bardaAe , MPI_INT, xixi, TAG_ALERT_ZARRAY,cart_comm);

          }

        }
      }
      //printf("LINE %d DONE\n", i);
    }

    /*termination array {-1, -1}*/
    coco[0] = -1;
    coco[1] = -1;
    for(i = 1; i < p; i++){
      MPI_Send(coco, 2 , MPI_INT, i, TAG_ALERT_XY, world->comm);
    }
  } else{
    MPI_Recv(blocklimits,4,MPI_INT,ROOT,TAG_ALERT_SIZE,cart_comm,&status);
    get_sizes_by_limits(blocklimits,coco);
    world_init(coco[0],coco[1],cube_size, blocklimits,id,cart_comm,dims_size,p);
    printf("[%d] world size %d %d\n",world->pID,world->sizeX,world->sizeY);

    for(;;){
      memset(world->zArray, -1, world->sizeZ);
      MPI_Recv(coco, 2, MPI_INT, ROOT, TAG_ALERT_XY, cart_comm, &status);
      if(coco[0] == -1 || coco[1] == -1)
        break;
      MPI_Recv(world->zArray,world->sizeZ,MPI_INT,ROOT,TAG_ALERT_ZARRAY,cart_comm,&status);

      world->cell_matrix[coco[0] - world->smallWorldLimits[0]][coco[1] - world->smallWorldLimits[2] ] = arrayToList(coco[0],coco[1],world->zArray,world->sizeZ, world->sizeZ, 0);
    }
    

  }

//printf("%d %d \n smallWorldFrontiers x[%d %d] y[%d %d] ", world->sizeX, world->sizeY, world->smallWorldLimits[0], world->smallWorldLimits[1], world->smallWorldLimits[2],world->smallWorldLimits[3]);
  for(i = 0; i < generations;i++){
   // printf("[%d] [%d] FETCH_start\n",world->pID,i);
    MPI_Barrier(world->comm);
    world->fetch_borders();
  //  printf("[%d] [%d] FETCH_DONE\n",world->pID,i);
    world->map();
  //  printf("[%d] [%d] MAPDONE\n",world->pID,i);
    world->get_next_gen();
    world->update_gen();
  }
  i = 1;

  world->order();
  MPI_Barrier(world->comm);

  for(xy[0] = 0; xy[0] < dims_size[0]; xy[0]++){
    for(a = 0; a < world->sizeX; a++){
      for(xy[1] = 0; xy[1] < dims_size[1]; xy[1]++){
        MPI_Cart_rank(world->comm, xy, &rank);
        if(rank == world->pID){
          world->print_line(a);
        }  
        MPI_Barrier(world->comm);
      }
    }

  }


#ifdef DEBUG_TIME
  end_t = clock();
  total_t = (double)( (end_t - start_t) / CLOCKS_PER_SEC );
  if(world->pID == ROOT)
    fprintf(stdout, "Total time taken by CPU: %f\n",total_t);

#endif
  MPI_Finalize();
  exit(0);

}
