#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tree_funcs.h"

Exp_node * nodeConnect(Exp_node *parent, const char dest)
{
    assert(parent != NULL);
    
    Exp_node * new_node = (Exp_node *)calloc(1, sizeof(Exp_node));
    
    switch(dest)
    {
        case LEFT_SON:
                parent->l_son = new_node;
                break;
        case RIGHT_SON:
                parent->r_son = new_node;
                break;
    }

    new_node->parent = parent;
    return new_node;

}

int nodeCtor(Exp_node **node)
{
    *node = (Exp_node *)calloc(1, sizeof(Exp_node));
    return 0;
}

void printIn(const Exp_node * node)
{
    if (!node) 
        return;

    printf("(");

    if (node->l_son)
    {
        printIn(node->l_son);
        printf(")");
    }

    switch(node->type)
    {
        case OP:
            printf("%c", node->op_value);
            break;
        case NUM:
            printf("%g", node->dbl_value);
            break;
        case VAR:
            printf("%c", node->var_value);
            break;
        default:
            printf("Блэт");
            break;

    }
 
    if (node->r_son)
    {
        printIn(node->r_son);
        printf(")"); 
    }

    return;
}


////////////////////////////////////////////////////////////

int nodeCtor(Node **node)
{
    *node = (Node *)calloc(1, sizeof(Node));
    return 0;
}

Node * nodeConnect(Node *parent, const char dest)
{
    assert(parent != NULL);
    
    Node * new_node = (Node *)calloc(1, sizeof(Node));
    
    switch(dest)
    {
        case LEFT_SON:
                parent->l_son = new_node;
                break;
        case RIGHT_SON:
                parent->r_son = new_node;
                break;
        default:
                //letim
                break;
    }

    new_node->parent = parent;
    return new_node;
}

void printPre(const Node * node)
{
    if (!node) 
        return;

    printf("( ");
    printf("%s ", node->data);

    if (node->l_son)
    {
        printPre(node->l_son);
        printf(") ");
    }

    if (node->r_son)
    {
        printPre(node->r_son);
        printf(") "); 
    }

    return;
}

void printIn(const Node * node)
{
    if (!node) 
        return;

    printf("( ");

    if (node->l_son)
    {
        printIn(node->l_son);
        printf(") ");
    }

    printf("%s ", node->data);

    if (node->r_son)
    {
        printIn(node->r_son);
        printf(") "); 
    }

    return;
}

void printPost(const Node * node)
{
    if (!node) 
        return;

    printf("( ");

    if (node->l_son)
    {
        printPost(node->l_son);
        printf(") ");
    }

    if (node->r_son)
    {
        printPost(node->r_son);
        printf(") "); 
    }

    printf("%s ", node->data);

    return;
}

Node * findNode(Node *node, const char *string)
{

    Node *result = nullptr;

    if (!node) 
        return result;

    if (strcmp(string, node->data) == 0)
    {
        result = node;
        return result;
    }

    if (node->l_son)
    {
        result = findNode(node->l_son, string);
    }

    if (node->r_son && result == nullptr)
    {
        result = findNode(node->r_son, string);
    }

    return result;

}

int nodeDtor(Node *node)
{
    if (!node)
        return 0;

    while(1)
    {
        
        // printf("\e[0;32mдолжны были зафришеть - \e[0m\n");
        // nodeDump(node);
        if (node->l_son == nullptr && node->r_son == nullptr)
        {
            if (node->parent == nullptr)
                return 0;

            if (node->parent->l_son == node)
            {
                node->parent->l_son = nullptr;
                    
            }
            else if(node->parent->r_son == node)
            {
                node->parent->r_son = nullptr;
            }

            // printf("\e[0;31mзафришен - \e[0m\n");
            // nodeDump(node);
            free((void *)node->data);
            free(node);
            return 0;

        }

        nodeDtor(node->l_son);

        nodeDtor(node->r_son);
    }

    return 0;
}