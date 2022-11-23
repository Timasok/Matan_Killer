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
    
/*
    Exp_node * simplified = simplifyTree(func->l_son); 

    treeDump(simplified);

    printIn(simplified);
    printf("\n");

    nodeDtor(simplified);

*/
    Exp_node * diff_func = differentiate(func->l_son);
    // Exp_node * diff_func = copy(func->l_son);
    
    treeDump(diff_func);

    printIn(diff_func);
    printf("\n");

    nodeDtor(diff_func);
    free(diff_func);// because we create null node that we link everything with

    nodeDtor(func);
    free(func);
    
    textDtor(&text);

    return 0;
}