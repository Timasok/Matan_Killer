/*
    (-1, +) means that this operation has 2 operands, and can be easily
    converted into arithmetical operation

    (-2, +) is for not operation

    (-3, +) is for ^

    + везде это костыль 

*/

DEF_OP(NOT_OP, '\r', -2, +)

DEF_OP(ADD, '+', -1, +)

DEF_OP(SUB, '-', -1, -)
    
DEF_OP(MUL, '*', -1, *)

DEF_OP(DIV, '/', -1, /)

DEF_OP(POW, '^', -3, +)

DEF_OP(SIN, 'S', 0, +)

DEF_OP(LN , 'L', 1, +)

DEF_OP(COS, 'C', 2, +)

DEF_OP(TAN, 'T', 3, +)

DEF_OP(SH, 's', 4, +)

DEF_OP(CH, 'c', 5, +)

DEF_OP(TH, 't', 6, +)

DEF_OP(ARCTG, '+', 7, +)

DEF_OP(ARCCOS, 'W', 8, +)