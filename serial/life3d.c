#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/defs.h"
#include "../common/error.h"
#include "../map/cell.h"
#include "../map/world.h"


int main(int argc, char * argv[]){

  int generations, cube_size;
  pos_ pos;
  int i;
  char line[MAX_LINE_SIZE];
  FILE * inputFile;
  world_stct *world;

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

  world = init_world( cube_size);

  for(i=0; fgets(line,MAX_LINE_SIZE,inputFile) ;i++){
    if (!sscanf(line,"%d %d %d", &pos.x,&pos.y,&pos.z))
      error_exit("Error: Invalid position",ERR_INVALID_POS);
    world->cell_list = insert_new_cell(world->cell_list,pos,cube_size -1);
    cell_update_state(world->cell_list);
  }
  for(i = 0; i < generations;i++){
    printf("%d\n",i);
#ifdef DEBUG
    printf("---- Generation %d: ----------\n",i);
    cell_list_print(world->cell_list);
    printf("\n");
#endif
  world = world_map(world);
  world = world_update_state(world);
  }
  world->cell_list = cell_order(world->cell_list);
  cell_list_print(world->cell_list);
  exit(0);
}
