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
  cell_tmp * cell_ptr, * aux;
  mini_world * first;
  mini_world * world;
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
  cell_ptr = NULL;
  for(i=0; fgets(line,MAX_LINE_SIZE,inputFile) ;i++){

    if (!sscanf(line,"%d %d %d", &x,&y,&z)){
      error_exit("Error: Invalid position",ERR_INVALID_POS);
    }
    cell_ptr = add_cell_tmp(cell_ptr,x,y,z);
  }

  for(i=0; aux = cell_ptr; aux != NULL;i++; aux = aux->next){
    world = miniworld_new(cube_size,aux)
    if( i == 0) first = world;
    world->add_cell(x,y,z)
  }

}
