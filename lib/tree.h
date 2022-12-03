#ifndef TREE_FUNCS_H
#define TREE_FUNCS_H

// #define DEBUG 47

const int MAX_BUFFER_LENGTH = 512;
const int NUMBER_OF_VARS = 16; 

#define ASSERT(condition)                                               \
    do{                                                                 \
        if (!condition)                                                 \
        {                                                               \
            fprintf(stderr, "%s failed: file: %s func: %s line: %d\n",  \
                     #condition, __FILE__, __FUNCTION__, __LINE__);     \
        }                                                               \
    } while(0)                                                          \

struct Node
{
    const char *data;
    Node * l_son = nullptr;
    Node * r_son = nullptr;
    Node * parent = nullptr;

};

enum son
{
    LEFT_SON   = 0,
    RIGHT_SON  = 1,
    HOLLOW = -1,
};

enum Node_type
{
    NUL = 0,
    OP  = 1,
    VAR = 2,
    NUM = 3,

};

#define DEF_OP(op_name, priority, op_code, num, oper, str_for_tex)                 \
    op_name = op_code,                                      \

enum Operator
{
    #include "operations.h"

};
  
#undef DEF_OP


typedef struct Var_t
{
    char *name;
    double value;
} Var;

union Value
{
    Operator op_value;
    double dbl_value;
    // char var;
    Var var;

};

struct Exp_node
{

    Exp_node * l_son = nullptr;
    Exp_node * r_son = nullptr;
    Exp_node * parent = nullptr;

    Node_type type = NUL;

    Value value;
    
};

Exp_node * nodeConnect(Exp_node *parent, const char dest);
Exp_node * nodeConnect(Node_type type, Value value, Exp_node * l_son, Exp_node * r_son);
Exp_node * nodeCtor();


int getVarIndex(Var v_arr[], const char * name);
int getVarValue(Var v_arr[], const char * name);
int addValueToVarArray(Var v_arr[], double value, const char *name);
int addVarValueByIndex(Var v_arr[], double value, size_t index);
int addVarNameByIndex(Var v_arr[], char * name, size_t index);

int fillVarArray(Var v_arr[], Exp_node * node, size_t free_index);
int dumpVarArray(Var v_arr[]);
int varArrayDtor(Var v_arr[]);

Exp_node * createNode(Node_type type, Value value, Exp_node * l_son, Exp_node * r_son);
Exp_node * createNum(double number);
Exp_node * createVar(char var);
Exp_node * createVar(char * var_name);
Exp_node * createOp(Operator op);
Exp_node * createOp(int operation);

int substitudeVariables(Exp_node *node, Var v_arr[], const char * name_of_not_replaced_var);
int processVarNode(Exp_node *node, Var v_arr[], const char * var_name);
int fillVarValues(Var v_arr[]);
bool checkExistence(Var v_arr[], const char * name);

#define CREATE_OP(op_name)          \
    createOp( ##op_name)

int linkToParent(Exp_node *parent, Exp_node *orphan);
int linkSonsToParent(Exp_node *node);
int pickCubs(Exp_node * prev_parent, Exp_node * new_parent);
// int tearNode(Exp_node * prev_parent, Exp_node * new_parent);

bool isTerminal(Exp_node *node);
bool hasSons(Exp_node *node);

Exp_node * copy(const Exp_node * node);
int copyNodeData(const Exp_node *src_node, Exp_node *dest_node);
int copySingle(const Exp_node * node, Exp_node *new_node);

Exp_node * findNode(Exp_node *node, const char *string);//TODO change completely this function do we need her?
int nodeDtor(Exp_node *node);
int nodeDtor(Exp_node **node);

void printPre(const Exp_node * node);
void printIn(const Exp_node * node);
void printPost(const Exp_node * node);

///////////////////////////////////////////////////////////////

int verifyTree(const Exp_node *node);

///////////////////////////////////////////////////////////////

int nodeCtor(Node **node);
// Node * nodeConnect(Node *parent, const char dest);

Node * findNode(Node *node, const char *string);
int nodeDtor(Node *node);

void printPre(const Node * node);
void printIn(const Node * node);
void printPost(const Node * node);

#endif