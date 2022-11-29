#include <stdio.h>
#include <stdlib.h>

#include "text_funcs.h"
#include "tree.h"
#include "matan_killer_f.h"
#include "matan_killer_debug.h"
#include "calc_f.h"

int main()
{
    Var v_arr[NUMBER_OF_VARS] = {};

    // getVarIndex(v_arr, "x");
    // getVarIndex(v_arr, "x");
    // getVarIndex(v_arr, "y");
    // getVarIndex(v_arr, "z_1");
    // getVarIndex(v_arr, "z_2");

    // dumpVarArray(v_arr);
    
    openLogs();
    Text_info text1 = {};
    
    textCtor(&text1, "expression.input");

    printText(&text1);

    Exp_node * result = getGeneral(text1.buf);

    printIn(result);
    printf("\n");

    TREE_DUMP(result);

/*
    Exp_node * diff_func = differentiate(result);
    simplifyTree(&diff_func);

    Exp_node * diff_copy = differentiate(diff_func);
    simplifyTree(&diff_copy);
        
    TREE_DUMP_OPTIONAL(diff_func, "after differentiation");
    printIn(diff_func);
    printf("\n");

    nodeDtor(diff_func);
    free(diff_func);

    TREE_DUMP_OPTIONAL(diff_copy, "after double differentiation");
    printIn(diff_copy);
    printf("\n");

    nodeDtor(diff_copy);
    free(diff_copy);
*/
    
    nodeDtor(result);
    free(result);

    textDtor(&text1);

    openPDF();
    closeLogs();
    
    varArrayDtor(v_arr);
    
    return 0;
}