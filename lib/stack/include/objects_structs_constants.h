//this header contains all objects and direct operations with 'em
#ifndef OBJ_STRUCTS_CONSTS_H
#define OBJ_STRUCTS_CONSTS_H

#include <stdio.h>

typedef unsigned long long canary_t;

typedef int elem_t;

// extern FILE * stderr = fopen("../../stderr.txt","w");

const elem_t POISON = 0xFFFFFFFF;

enum Stack_Error 
{
    STACK_ERROR_NULL                     = 1 << 0,
    STACK_ERROR_SIZE_BELOW_ZERO          = 1 << 1,
    STACK_ERROR_CAPACITY_BELOW_ZERO      = 1 << 2,
    STACK_ERROR_OVERSIZED                = 1 << 3,
    STACK_ERROR_SIZE_T_OVERFLOW          = 1 << 4,
    // STACK_ERROR_LEFTSTRUCT_CANARY_DIED   = 1 << 5,
    // STACK_ERROR_RIGHTSTRUCT_CANARY_DIED  = 1 << 6,
    // STACK_ERROR_LEFTBUF_CANARY_DIED      = 1 << 7,
    // STACK_ERROR_RIGHTBUF_CANARY_DIED     = 1 << 8,
    STACK_ERROR_INVALID_POINTER          = 1 << 9,
    STACK_ERROR_POINTER_IS_NULL          = 1 << 10,
    STACK_ERROR_DOUBLE_DTOR_INIT         = 1 << 11,
    STACK_ERROR_DOUBLE_CTOR_INI          = 1 << 12,
    // STACK_ERROR_WRONG_BUF_HASH           = 1 << 13,
    // STACK_ERROR_WRONG_HASH               = 1 << 14,
    // STACK_ERROR_SET_BUF_CANARY           = 1 << 15,
    STACK_IS_CORRECT                     = 0,
};

struct Var_Info
{
    const char * name_of_var;
    const char * name_of_file;
    const char * name_of_func;
    int number_of_line;
};

struct Stack
{

    elem_t * data;
    size_t size;
    size_t capacity;
    unsigned int code_of_error; 

    long buf_hash;    
    long hash;

    Var_Info var_info;

};

const int STACK_RESIZE_IF_POP          = 4;
const int STACK_RESIZE_IF_PUSH         = 2;
const char POISON_BORDER[] = "----------------------POISON BORDER ------------------------\n";

#endif