#include <stdio.h>
#include <stdlib.h>

#include "tree_funcs.h"
#include "matan_killer_f.h"
#include "matan_killer_debug.h"

int main()
{
    Exp_node * body = {};
    nodeCtor(&body);

    getExpression(body);

    printIn(body->l_son);
    printf("\n");

    treeDump(body);

    nodeDtor(body);
    free(body);

    return 0;
}