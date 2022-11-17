#include "objects_structs_constants.h"

#ifndef DEBUG_FUNCS_H
#define DEBUG_FUNCS_H

#define PRINT_ERR(...)                                                  \
        do {                                                            \
            fprintf(stderr,"\e[0;31mERROR: " );                         \
            fprintf(stderr, __VA_ARGS__);                               \
            fprintf(stderr,"\e[0m");                                    \ 
            } while(0)                                                  \


void hardAssert(bool condition); 
// void verifyStack(Stack * stk, int mode);

const char * stackStrError(Stack *stk);
int returnStackError(Stack *stk);

void printVarInfo(Stack *stk, FILE *output);

void stackDump(Stack *stk, FILE *output, const char * name_of_file, const char * name_of_func, int number_of_line);                                   

#endif