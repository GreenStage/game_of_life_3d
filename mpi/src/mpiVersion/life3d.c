#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "../common/defs.h"
#include "../common/error.h"
#include "../map/world.h"
#ifdef DEBUG_TIME
#include <time.h>
#endif
#define TAG_ALERT_SIZE 1
#define TAG_ALERT_XY 2
#define TAG_ALERT_ZARRAY 3
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
  printf("BAC : %d - %d \n",i[0],i[1]);
  return (MPI_ERR_TOPOLOGY == MPI_Cart_rank(comm,i,&retval) ) ? -1 : retval;
}

int get_block_limits(int block_owner,int cube_size,int dim_size, int retval[4], MPI_Comm comm){
  int dim_cords[2];
   int normSizeX, normSizeY;

  MPI_Cart_coords(coom,block_owner,2,dim_cords);

  normSizeX = cube_size / dim_size[0];
  normSizeY = cube_size / dim_size[1];

  retval[0] = normSizeX * dim_cords[0];
  retval[1] = (dim_cords[0] == dim_size[0] - 1) ? cube_size : normSizeX * (dim_cords[0] + 1) ;

  retval[2] = normSizeY * dim_cords[1];
  retval[3] = (dim_cords[1] == dim_size[1] - 1) ? cube_size : normSizeY * (dim_cords[1] + 1) ;

  return 0;
}

int get_id_by_dims_pos(int it,int dims_size){
  MPI_Cart_coords(comm,)
}
int main(int argc, char * argv[]){

  int generations, cube_size;
  pos_ pos;
  int a,i;
  int id, p;
  int partSizeX, partSizeY;
  int dims_size[2];
  int periodic[2] = {0,0};
  int blocklimits[4];
  int arraySize;
  int coco[2],xixi,bardaAe;  
  int * bufZ;
  MPI_Comm cart_comm;

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

  inputFile = fopen(argv[1], "r");
  if(inputFile == NULL){
    error_exit("Error: Cant open %s for reading",ERR_OPEN_FILE);
  }

  fgets(line,MAX_LINE_SIZE,inputFile);
  if (!sscanf(line,"%d", &cube_size)){
    error_exit("Error: Invalid cube size",ERR_INVALID_SIZE);
  }

  world = NULL;
  world_init( cube_size );

  /*Initialize MPI*/
  MPI_Init (&argc, &argv);
  MPI_Barrier(MPI_COMM_WORLD);
  
  MPI_Comm_rank (MPI_COMM_WORLD, &id); /*Get process rank*/
  MPI_Comm_size (MPI_COMM_WORLD, &p); /*Get number of processes*/

  /*Create Grid*/
  dims_size[0] = dims_size[1] = 0;
  MPI_Dims_create(p,2,dims_size);
  MPI_Cart_create(MPI_COMM_WORLD, 2, dims_size, periodic, 1, &cart_comm);
  
  pos.x = 499;
  pos.y = 0;
  pos.z = 199;
  printf("%d",block_owner(cube_size,pos,dims_size,cart_comm));
  printf("\n cartworld %d %d \n",dims_size[0],dims_size[1]);


  if( id == ROOT ){
    temp_pos = (pos_ *) malloc(sizeof(pos_) * cube_size);

    for(a=0; fgets(line,MAX_LINE_SIZE,inputFile) ;a++){
      if (!sscanf(line,"%d %d %d", &pos.x,&pos.y,&pos.z)){
        error_exit("Error: Invalid position",ERR_INVALID_POS);
      }

      world->add_cell(pos);
      
      if(ROOT == block_owner(cube_size,pos,dims_size,cart_comm))
        world->add_cell(pos);

    }

    for(a = 1; a < p; a++){
      get_block_limits(a,cube_size,dim_size,blocklimits, cart_comm);
      MPI_Send(blocklimits, 4 , MPI_INT, a, TAG_ALERT_SIZE,cart_comm);
    }

    for(i = 0; i < cube_size; i++){
      for(a = 0; a < cube_size; a++){

        bufZ = world->list_to_arr(bardaAe,i, a);

        if(bardaAe > 0){
          coco[0] = i;
          coco[1] = a;
          MPI_Send(coco, 2 , MPI_INT, get, TAG_ALERT_XY,cart_comm);
          MPI_Send(bufZ, bardaAe , MPI_INT, get, TAG_ALERT_ZARRAY,cart_comm);
        }
      }
    }
  }
  else{
    recevie do root();
 MPI_Send(coco, 2 , MPI_INT, get, TAG_ALERT_XY,cart_comm);
          MPI_Send(bufZ, bardaAe , MPI_INT, get, TAG_ALERT_ZARRAY,cart_comm);
  }





  MPI_Finalize();


  for(i = 0; i < generations;i++){
#ifdef DEBUG
    printf("---- Generation %d: ----------\n",i);
    printf("\n");
#endif

    world->map();
    world->get_next_gen();
    world->update_gen();
  }

  world->order();
  world->print(stdout);
  world->destroy();

#ifdef DEBUG_TIME
  end_t = clock();
  total_t = (double)( (end_t - start_t) / CLOCKS_PER_SEC );
  fprintf(stdout, "Total time taken by CPU: %f\n",total_t);

#endif

  exit(0);
  
}
