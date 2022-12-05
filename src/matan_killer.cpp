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

    Var v_arr[NUMBER_OF_VARS] = {};
    fillVarArray(v_arr, result, 0);
    // dumpVarArray(v_arr);

    TREE_DUMP_OPTIONAL(result, "initial tree"); 
    printIn(result);
    printf("\n");
    
    processSelectedMode(result, v_arr);
    
    nodeDtor(&result);
    textDtor(&text1);
    varArrayDtor(v_arr);

    openPDF();

    closeLogs();

    return 0;
}