#include <stdio.h>
#include <stdlib.h>

#include "text_funcs.h"
#include "tree.h"
#include "matan_killer_f.h"
#include "matan_killer_debug.h"

int main()
{
    openLogs();

    Exp_node * func = nodeCtor();
    
    Text_info text = {};
    
    textCtor(&text, "expression.input");

    printText(&text);

    getExpression(&text, func);
    printIn(func->l_son);
    printf("\n");
    TREE_DUMP(func->l_son);

    Exp_node * simplified = func->l_son;
    simplified = simplifyTree(simplified);

    TREE_DUMP_OPTIONAL(simplified, "simplified");

    printIn(simplified);
    printf("\n");

    nodeDtor(simplified);


/*    Exp_node * diff_func = differentiate(func->l_son);
    // Exp_node * diff_func = copy(func->l_son);
    
    TREE_DUMP(diff_func);

    printIn(diff_func);
    printf("\n");

    nodeDtor(diff_func);
    free(diff_func);// because we create null node that we link everything with
*/

    nodeDtor(func);
    free(func);
    
    textDtor(&text);

    closeLogs();

    return 0;
}