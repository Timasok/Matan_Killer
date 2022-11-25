#include <stdio.h>
#include <stdlib.h>

#include "text_funcs.h"
#include "tree.h"
#include "matan_killer_f.h"
#include "matan_killer_debug.h"
#include "calc_f.h"

int main()
{   
    openLogs();
    Text_info text1 = {};
    
    textCtor(&text1, "expression.input");

    printText(&text1);

    Exp_node * result = getGeneral(text1.buf);
    printf("%s = %p\n", text1.buf, result);    

    printIn(result);
    printf("\n");

    TREE_DUMP(result);

    nodeDtor(result);
    free(result);

    textDtor(&text1);

    closeLogs();
/*
// {
//     

//     Text_info text = {};   
//     textCtor(&text, "expression.input");
//     printText(&text);

// {
//     Exp_node * func = nodeCtor();
//     getExpression(&text, func);

//     TREE_DUMP_OPTIONAL(func->l_son, "initial tree");
//     printIn(func->l_son);
//     printf("\n");

//     // Exp_node * copy_initial = copy(func->l_son);
        
//     // // copy_initial = wrapEquivalents(copy_initial);       
//     // // computeConstants(copy_initial);

//     // TREE_DUMP_OPTIONAL(copy_initial, "initial after simplification");
//     // printIn(copy_initial);
//     // printf("\n");

//     // nodeDtor(copy_initial);
//     // free(copy_initial);


// {
//     Exp_node * diff_func = differentiate(func->l_son);

//     TREE_DUMP_OPTIONAL(diff_func, "after differentiation");
//     printIn(diff_func);
//     printf("\n");

// {   
//     Exp_node * copy_node = copy(diff_func);
        
//     wrapEquivalents(copy_node);       
//     computeConstants(copy_node);

//     TREE_DUMP_OPTIONAL(copy_node, "final after simplification");
//     printIn(copy_node);
//     printf("\n");

//     nodeDtor(copy_node);
//     free(copy_node);
// }

//     nodeDtor(diff_func);
//     free(diff_func);

// }

//     nodeDtor(func);
//     free(func);

// }
    
//     textDtor(&text);


// }
*/    
    return 0;
}