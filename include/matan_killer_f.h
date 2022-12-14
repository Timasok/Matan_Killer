#ifndef MATAN_KILLER_H
#define MATAN_KILLER_H

#include "tree.h"
#include "text_funcs.h"

enum OPERATION_PRIORITIES
{
    UNAR_OP    = 1,
    POW_OP     = 2,
    MUL_DIV_OP = 3,
    ADD_SUB_OP = 4,
};

enum modes
{
    DIFF                = 1,
    DIFF_PARTIALY       = 2,
    COMPUTE_IN_POINT    = 3,
    ARRANGE_IN_A_ROW    = 4,
    SIMPLIFY            = 5,
    QUIT                = 6,
};

const int MAX_OPER_LEN = 10;
const int MAX_FUNC_NUMBER = 30; 
const int MAGIC_NUMBER_THAT_STANDS_FOR_ARITHM = -1;
const int MAGIC_NUMBER_THAT_STANDS_FOR_POW = -3;

struct  Lex_sub
{
    char *initial;
    char parsed[5];

};

int LexDtor(Lex_sub *lex);
Lex_sub * getLexicalSubstitusions();

char * replaceFuncNames(const char * input);


son equalsZero(Exp_node *node);
son equalsOne(Exp_node *node);

Operator isOp(int symbol);
bool hasNumSons(Exp_node *node);

int hasVariable(const Exp_node *node);

double arrangeInTeilorRow(const Exp_node *node, Var v_arr[], size_t depth);
double calculateTreeFilled(Exp_node *node, Var v_arr[], const char * var_name, double x0);

int processSelectedMode(Exp_node * initial_tree, Var v_arr []);

int getExpression(Text_info *text, Exp_node *main_node);
int readExpression(Exp_node *exp_node, const char * remainder, size_t shift, int free_port);

int parseTerminalNode(Exp_node *exp_node, const char * parsing_start, size_t parsing_length);

int writeInColor(const char * sentence, char color);

Exp_node * differentiate_n_times(const Exp_node *node, size_t * number);

Exp_node * differentiate(const Exp_node *exp_node);
Exp_node * differentiatePartialy(const Exp_node *node, Var v_arr[]);

double calculateTree(Exp_node *node, Var v_arr[]);
int calculateNumTree(Exp_node *node);

Exp_node * simplifyTreeRolls(Exp_node *node);

Exp_node * makeLNTree(const Exp_node * argument);
Exp_node * makePowTree(const Exp_node * left_argument, double number);

int simplifyTree(Exp_node **node);
int wrapEquivalents(Exp_node *node);
int computeConstants(Exp_node *node);

int rollUpTree(Exp_node *node);
int processOneZeroCases(Exp_node *node);

int diffNode(const Exp_node *argument, Exp_node * result, const char linking_side_in_copy);
int copyNode(const Exp_node *argument, Exp_node * result, const char linking_side_in_copy);

int diffNode(const Exp_node *argument, Exp_node * result, const char linking_side_in_copy, const char src_side);
int copyNode(const Exp_node *argument, Exp_node * result, const char linking_side_in_copy, const char src_side);

#endif