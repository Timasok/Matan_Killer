#include <stdio.h>
#include <stdlib.h>

#include "text_funcs.h"
#include "tree_funcs.h"
#include "matan_killer_f.h"
#include "matan_killer_debug.h"

int main()
{
    Exp_node * body = {};
    nodeCtor(&body);

    Text_info text = {};
    
    textCtor(&text, "expression.input");

    printText(&text);

    getExpression(&text, body);

    printIn(body->l_son);
    printf("\n");

    treeDump(body);

    nodeDtor(body);
    
    textDtor(&text);
    free(body);

    return 0;
}