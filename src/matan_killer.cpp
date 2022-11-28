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

    printIn(result);
    printf("\n");

    TREE_DUMP(result);

    Exp_node * diff_func = differentiate(result);

    simplifyTree(&diff_func);
    
    TREE_DUMP_OPTIONAL(diff_func, "after differentiation");
    printIn(diff_func);
    printf("\n");

    nodeDtor(diff_func);
    free(diff_func);

    nodeDtor(result);
    free(result);

    textDtor(&text1);

    openPDF();
    closeLogs();
    
    return 0;
}