#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <errno.h>
#include <stdio.h>

#include "debug_funcs.h"

#define LOCATION(reason) fprintf(stderr, "%s: file: %s func: %s line: %d\n", reason, \
                                 __FILE__, __FUNCTION__, __LINE__)

void hardAssert(bool condition)//works the wrong way
{
   // if (!condition)
   // {
   //    fputs("CRITICAL ERROR\n", stderr);
   //    LOCATION("Terminated");
   //    abort();
   // }
}

// void verifyStack(Stack *stk, int mode)
// {
   
//    ASSERT_STK(stk, stderr);
   
// }

// #undef LOCATION(reason)

int returnStackError(Stack *stk)
{

   stk->code_of_error |= ((!stk->data) * STACK_ERROR_NULL);

   stk->code_of_error |= ((stk->size < 0) * STACK_ERROR_SIZE_BELOW_ZERO);

   stk->code_of_error |= ((stk->capacity < 0) * STACK_ERROR_CAPACITY_BELOW_ZERO);

   stk->code_of_error |= ((stk->size > stk->capacity) * STACK_ERROR_OVERSIZED);

   // stk->code_of_error |= ((!isLeftBufCanary(stk)) * STACK_ERROR_LEFTBUF_CANARY_DIED);

   // stk->code_of_error |= ((!isRightBufCanary(stk)) * STACK_ERROR_RIGHTBUF_CANARY_DIED);

   // stk->code_of_error |= ((!isLeftStructCanary(stk)) * STACK_ERROR_LEFTSTRUCT_CANARY_DIED);

   // stk->code_of_error |= ((!isRightStructCanary(stk)) * STACK_ERROR_RIGHTSTRUCT_CANARY_DIED);

   return stk->code_of_error;
}

const char *stackStrError(Stack *stk)
{
   // add light maybe
   char *result = (char *)calloc(100, sizeof(char));

   if (stk->code_of_error & STACK_ERROR_NULL)
      strcat(result, "Data pointer = NULL\n");

   if (stk->code_of_error & STACK_ERROR_SIZE_BELOW_ZERO)
      strcat(result, "Size < 0\n");

   if (stk->code_of_error & STACK_ERROR_CAPACITY_BELOW_ZERO)
      strcat(result, "Capacity < 0\n");

   if (stk->code_of_error & STACK_ERROR_OVERSIZED)
      strcat(result, "Size > capacity\n");

   // if (stk->code_of_error & STACK_ERROR_LEFTBUF_CANARY_DIED)
   //    strcat(result, "ERROR: LEFT BUF CANARY IS DEAD\n");

   // if (stk->code_of_error & STACK_ERROR_RIGHTBUF_CANARY_DIED)
   //    strcat(result, "ERROR: RIGHT BUF CANARY IS DEAD\n");

   // if (stk->code_of_error & STACK_ERROR_LEFTSTRUCT_CANARY_DIED)
   //    strcat(result, "ERROR: RIGHT STRUCT CANARY IS DEAD\n");

   // if (stk->code_of_error & STACK_ERROR_RIGHTSTRUCT_CANARY_DIED)
   //    strcat(result, "ERROR: RIGHT STRUCT CANARY IS DEAD\n");

   // if (stk->code_of_error & STACK_ERROR_WRONG_BUF_HASH)
   //    strcat(result, "ERROR: BUF HASH WAS UNPREDICTABLY CHANGED\n");

   // if (stk->code_of_error & STACK_ERROR_WRONG_HASH)
   //    strcat(result, "ERROR: HASH WAS UNPREDICTABLY CHANGED\n");

   if (stk->code_of_error == 0)
      strcat(result, "(ok)");

   return result;
}

//todo printElementType
static void printElement(Stack *stk, int index, FILE *output)
{
   fprintf(output, "\t*[%d] = %d\n", index, stk->data[index]); 

}

void printStack(Stack *stk, FILE *output)
{
   assert(output != NULL);
   
   fprintf(output, "{\n");
   fprintf(output, "size = %ld\n", stk->size);
   fprintf(output, "capacity = %ld\n", stk->capacity);
   fprintf(output, "data[%p]\n" , stk->data); 
   fprintf(output, "code_of_error = %d\n", stk->code_of_error);
   fprintf(output, "{\n\t{\n");

   int counter = stk->capacity;
   while(--counter >= 0)
   {
      if(counter == stk->size - 1)
         fputs(POISON_BORDER, output);

      printElement(stk, counter, output);

   }

   fprintf(output, "\t}\n}\n");
   
}

void printVarInfo(Stack *stk, FILE *output)
{
   assert(output != NULL);

   fprintf(output, "Stack[%p] "
                     "%s"
                     " at %s at %s(%d)\n",
           stk,
           stk->var_info.name_of_var, stk->var_info.name_of_func,
           stk->var_info.name_of_file, stk->var_info.number_of_line);

}

void stackDump(Stack *stk, FILE *output, const char *name_of_file, const char *name_of_func,
                      int number_of_line)
{
   assert(output != NULL);

   PRINT_ERR("%s at %s(%d)\n", name_of_func, name_of_file, number_of_line);

   PRINT_ERR("%s", stackStrError(stk));

   printVarInfo(stk, output);

   printStack(stk, output);

}

#undef ASSERT_STK