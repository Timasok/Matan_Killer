#include <stdio.h>
#include <stdlib.h>

#include "text_funcs.h"
#include "tree.h"
#include "matan_killer_f.h"
#include "matan_killer_debug.h"

int main()
{
    Exp_node * func = nodeCtor();
    
    Text_info text = {};
    
    textCtor(&text, "expression.input");

    printText(&text);

    getExpression(&text, func);
    printIn(func->l_son);
    printf("\n");
    treeDump(func->l_son);

    Exp_node * diff_func = differentiate(func->l_son);
    
    treeDump(diff_func);

    printIn(diff_func);
    printf("\n");

    nodeDtor(diff_func);
    free(diff_func);

    nodeDtor(func);
    
    textDtor(&text);
    free(func);

    return 0;
}