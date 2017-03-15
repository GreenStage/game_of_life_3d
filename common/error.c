#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void error_exit(char * errmsg, int errno){
  printf("%s \n",errmsg);

  if(errno < MAX_ARG_ERROR){
    printf("Usage: 'life3d <input file> <geneations>' \n");
    printf("       input file must exist in the same directory and have a .in extension, \n");
    printf("       generations must be a non-zero integer. \n");
  }
  
  exit(errno);
}
