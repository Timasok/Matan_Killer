/*
    (-1, +) means that this operation has 2 operands, and can be easily
    converted into arithmetical operation

    (-2, +) is for not operation

    (-3, +) is for ^

    + везде это костыль 

//TODO add constants such as PI and E
*/

DEF_OP(NOT_OP, 0, '\r', -2, +, "")

DEF_OP(ADD, 4, '+', -1, +, "+")

DEF_OP(SUB, 4, '-', -1, - , "-")
    
DEF_OP(MUL, 3, '*', -1, *, "\\cdot ")

DEF_OP(DIV, 3, '/', -1, /, "")

DEF_OP(POW, 2, '^', -3, +, "^")

DEF_OP(SIN, 1, 'S', 0, +, "sin")

DEF_OP(LN , 1, 'L', 1, +, "ln")

DEF_OP(COS, 1, 'C', 2, + , "cos")

DEF_OP(TAN, 1, 'T', 3, + , "tan")

DEF_OP(SH, 1, 's', 4, +, "sh")

DEF_OP(CH, 1, 'c', 5, +, "ch")

DEF_OP(TH, 1, 't', 6, +, "th")

DEF_OP(ARCTG, 1, '+', 7, +, "arctg")

DEF_OP(ARCCOS, 1,  'W', 8, +, "arccos")

DEF_OP(ARCSIN, 1, 'Q', 9, +, "arsin")

DEF_OP(EXP, 1,  'E', 10, +, "\\exp")

DEF_OP(SQRT, 1, 'F', 11, +, "\\sqrt")