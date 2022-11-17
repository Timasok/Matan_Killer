#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <errno.h>

#include "debug_funcs.h"

#define ASSERT_STK(stkPtr, output)                                      \
   do{                                                                  \
      if (returnStackError(stkPtr))                                     \
         stackDump(stkPtr, output, __FILE__, __FUNCTION__, __LINE__);   \
   } while (0)

static void * getStackElement(Stack * stk, int index)
{
    hardAssert(stk != NULL);
    return (stk->data + index);

}

static void setStackElement(Stack * stk, int index, elem_t element)
{
    hardAssert(element != 0);
    hardAssert(stk != nullptr);

    stk->data[index] = element;
      
}

static void fillWithPoison(Stack *stk, size_t size, size_t capacity)
{
    // memset(getStackElement(stk, size), POISON, capacity - size -1 );

    for (size_t counter = stk->size; counter < stk->capacity; counter++)
        setStackElement(stk, counter, POISON);
}

void stackResize(Stack * stk, size_t new_capacity)
{
    //ASSERT_STK(stk, stderr);

    if (new_capacity <= stk->capacity)
        return ;

    elem_t* tmp_ptr = (elem_t*)realloc(stk->data, new_capacity * sizeof(elem_t));

    hardAssert(tmp_ptr != NULL);

    stk->data = tmp_ptr;
    stk->capacity = new_capacity;

    fillWithPoison(stk, stk->size, stk->capacity);

    //ASSERT_STK(stk, stderr);
     
}

void stack_Ctor(Stack *stk, size_t capacity, const char * name_of_var, 
                                        const char * name_of_file, const char * name_of_func, int number_of_line)
{   
    //ASSERT_STK(stk, stderr);

    stk->var_info = {.name_of_var  = name_of_var, 
    .name_of_file = name_of_file, .name_of_func = name_of_func, .number_of_line  = number_of_line};

    // stk->var_info.name_of_var  = name_of_var;
    // stk->var_info.name_of_file = name_of_file;
    // stk->var_info.name_of_func = name_of_func;
    // stk->var_info.number_of_line  = number_of_line;

    hardAssert(stk != NULL);

    stk->data = (elem_t *)calloc(capacity , sizeof(elem_t));
    hardAssert(stk->data != NULL);

    stk->code_of_error = 0;
    stk->capacity = capacity;
    stk->size = 0;    

    fillWithPoison(stk, stk->size, stk->capacity);

    //ASSERT_STK(stk, stderr);

}

void stackPush(Stack *stk, elem_t element)
{
   
    //ASSERT_STK(stk, stderr);

    if (stk->size >= stk->capacity)
        stackResize(stk, stk->capacity * STACK_RESIZE_IF_PUSH); 

    // setStackElement(stk, stk->size, element);

    // fprintf(stderr, "%ld\n", stk->size);
    stk->data[stk->size++] = element;

    //ASSERT_STK(stk, stderr);

}

void stackDtor(Stack *stk)
{
    //ASSERT_STK(stk, stderr);

    fillWithPoison(stk, stk->size, stk->capacity);
    free((void *)stk->data);

}

int stackPop(Stack *stk, elem_t * element) 
{
    //ASSERT_STK(stk, stderr);

    if (stk->size == 0)
    {
        PRINT_ERR("You attained the end of stack\n");
        stackDtor(stk);
        return EXIT_FAILURE;
    }

    if (stk->size * STACK_RESIZE_IF_POP < stk->capacity)
        stackResize(stk, stk->capacity);

    // *element = *(elem_t *)getStackElement(stk, stk->size-1);
    // setStackElement(stk,--stk->size, POISON);
    // fprintf(stderr, "%ld\n", stk->size);
    *element = stk->data[stk->size-1];
    stk->data[--stk->size] = POISON;

    //ASSERT_STK(stk, stderr);

    return EXIT_SUCCESS;
}


#undef ASSERT_STK