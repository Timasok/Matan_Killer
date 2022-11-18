#ifndef TREE_FUNCS_H
#define TREE_FUNCS_H

// #define DEBUG 47

const int MAX_BUFFER_LENGTH = 512;

struct Node
{
    const char *data;
    Node * l_son = nullptr;
    Node * r_son = nullptr;
    Node * parent = nullptr;

};

enum son
{
    LEFT_SON  = 0,
    RIGHT_SON = 1,
};

enum Node_type
{
    NUL = 0,
    OP  = 1,
    VAR = 2,
    NUM = 3,

};

enum Operator
{
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '\\',

};
  
struct Exp_node
{

    Exp_node * l_son = nullptr;
    Exp_node * r_son = nullptr;
    Exp_node * parent = nullptr;

    Node_type type = NUL;

    Operator op_value;
    double dbl_value;
    char var_value;

};

Exp_node *  nodeConnect(Exp_node *parent, const char dest);
int nodeCtor(Exp_node **node);
Exp_node * nodeConnect(Exp_node *parent, const char dest);

Exp_node * findNode(Exp_node *node, const char *string);//TODO change completely this function do we need her?
int nodeDtor(Exp_node *node);

void printPre(const Exp_node * node);
void printIn(const Exp_node * node);
void printPost(const Exp_node * node);

///////////////////////////////////////////////////////////////

int nodeCtor(Node **node);
Node * nodeConnect(Node *parent, const char dest);

Node * findNode(Node *node, const char *string);
int nodeDtor(Node *node);

void printPre(const Node * node);
void printIn(const Node * node);
void printPost(const Node * node);

#endif