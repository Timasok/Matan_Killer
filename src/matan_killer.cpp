#include <stdio.h>
#include <stdlib.h>

#include "text_funcs.h"
#include "tree_funcs.h"
#include "matan_killer_f.h"
#include "matan_killer_debug.h"

int main()
{
    Exp_node * body = nodeCtor();
    
    Text_info text = {};
    
    textCtor(&text, "expression.input");

    printText(&text);

    getExpression(&text, body);
    printIn(body->l_son);
    printf("\n");

    Exp_node * copy_body = copyNode(body);
    
    treeDump(copy_body);

    printIn(copy_body->l_son);
    printf("\n");

    nodeDtor(copy_body);
    free(copy_body);

    treeDump(body->l_son);
    nodeDtor(body);
    
    textDtor(&text);
    free(body);

    return 0;
}