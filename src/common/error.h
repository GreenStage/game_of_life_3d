#ifndef ERROR_HEADER
#define ERROR_HEADER

#define ERR_MISSING_ARG 1
#define ERR_INVALID_INPUT 2
#define ERR_INVALID_GEN 3
#define MAX_ARG_ERROR 15

#define ERR_OPEN_FILE 16
#define ERR_INVALID_SIZE 17
#define ERR_INVALID_POS 18

void error_exit(char * errmsg, int errno);
#endif
