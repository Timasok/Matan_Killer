#include <stdio.h>

#include "tree_funcs.h"

int main()
{
    Exp_node * plus = {};
    nodeCtor(&plus);
    Exp_node * var_x = nodeConnect(plus, LEFT_SON);
    Exp_node * num_47 = nodeConnect(plus, RIGHT_SON);

    plus->type   = OP;
    plus->op_value = ADD;

    var_x->type      = VAR;
    var_x->var_value = 'x';

    num_47->type = NUM;
    num_47->dbl_value = 47.0;

    printIn(plus);

    return 0;
}