#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/defs.h"
#include "../common/error.h"
#include "../common/world.h"

int main(int argc, char * argv[]){

  int generations, cube_size;
  int x,y,z;
  int z;
  int i;
  char line[MAX_LINE_SIZE];
  FILE * inputFile;
  world_stct world;


  if(argc < 3){
    error_exit("Error: Missing argument",ERR_MISSING_ARG);
  }

  if( strcmp( ".in" , ( argv[1] + sizeof( argv[1] ) -4 ) ) != 0 ){
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

  world.cell_ptr = NULL;
  world.init( new_cell() );

  for(i=0; fgets(line,MAX_LINE_SIZE,inputFile) ;i++){

    if (!sscanf(line,"%d %d %d", &x,&y,&z)){
      error_exit("Error: Invalid position",ERR_INVALID_POS);
    }
    world.cell_ptr = world.add_cell(ptr,elements,x,y,z);
  }
  world_map(&world);

}
