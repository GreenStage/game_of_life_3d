#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/defs.h"
#include "../common/error.h"
#include "../map/cell.h"
#include "../map/world.h"
#ifdef DEBUG_TIME
#include <time.h>
#endif

int main(int argc, char * argv[]){

  int generations, cube_size;
  pos_ pos;
  int i;
  char line[MAX_LINE_SIZE];
  FILE * inputFile, * Outputfile;
  world_stct *world;
#ifdef DEBUG_TIME
  clock_t start_t, end_t, total_t;
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
  if (!(Outputfile = fopen("output.txt", "w")))
	  exit(1);

  world = init_world( cube_size);

  for(i=0; fgets(line,MAX_LINE_SIZE,inputFile) ;i++){
    if (!sscanf(line,"%d %d %d", &pos.x,&pos.y,&pos.z))
      error_exit("Error: Invalid position",ERR_INVALID_POS);
    world_add_cell(world,pos);
  }


  for(i = 0; i < generations;i++){
    printf("%d\n",i);
#ifdef DEBUG
    printf("---- Generation %d: ----------\n",i);
    printf("\n");
#endif

    world_map(world);  
    world_get_next_gen(world);
    world_update_gen(world);
  }
  world_order(world);
  world_print(world,Outputfile);
  fclose(Outputfile);

#ifdef DEBUG_TIME
  end_t = clock();
  total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  fprintf(Outputfile, "Total time taken by CPU: %f\n", (double) total_t);
#endif
  	  exit(0);
  exit(0);
}
