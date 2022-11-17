#ifndef STACK_FUNCS_H
#define STACK_FUNCS_H

#include "../include/objects_structs_constants.h"

#define stackCtor(stk, capacity) stack_Ctor(&stk, capacity, #stk,                     \
                                            __FILE__, __FUNCTION__,__LINE__)

void stack_Ctor(Stack *stk, size_t capacity, const char * name_of_var, const char * name_of_file, 
                                                                    const char * name_of_func, int number_of_line);
void stackDtor(Stack *stk);

void stackPush(Stack *stk, elem_t element);
int stackPop(Stack *stk, elem_t * element);
void stackResize(Stack * stk, size_t new_capacity);

void printStack(Stack *stk, FILE *output);

#endif