#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tree.h"
#include "matan_killer_debug.h"//TODO debuger is external

Exp_node * nodeConnect(Exp_node *parent, const char dest)
{
    assert(parent != NULL);

#ifdef DEBUG
    printf("\e[0;31mconnect_called to %p dest %d\e[0m\n", parent, dest);
#endif

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

Exp_node * nodeCtor()
{
    return (Exp_node *)calloc(1, sizeof(Exp_node));
}

/* Exp_node * copy(Exp_node * node)
{
    if (!node)
        return nullptr;
    
    Exp_node *new_node = nodeCtor();

    copyNodeData(node, new_node);

    if (node->l_son)
    {
        new_node->l_son = copy(node->l_son);
        new_node->l_son->parent = new_node;
    }

    if (node->r_son)
    {
        new_node->r_son = copy(node->r_son);
        new_node->r_son->parent = new_node;
    }

#ifdef DEBUG
    dumpExpNode(new_node);
#endif

    return new_node;

}
*/

//TODO clear
Exp_node * copy(Exp_node * node)
{
    if (!node)
        return nullptr;
    
    Exp_node *new_node = nodeCtor();

    copyNodeData(node, new_node);

#ifdef DEBUG
    dumpExpNode(new_node);
#endif

    return new_node;

}

int copyNodeData(const Exp_node *src_node, Exp_node *dest_node)
{
    if (src_node == nullptr || dest_node == nullptr)
        return -1;


    dest_node->type = src_node->type;
    dest_node->value = src_node->value;

    if (src_node->l_son)
    {
        nodeConnect(dest_node, LEFT_SON);
        copyNodeData(src_node->l_son, dest_node->l_son);
        dest_node->l_son->parent = dest_node;
    }

    if (src_node->r_son)
    {
        nodeConnect(dest_node, RIGHT_SON);
        copyNodeData(src_node->r_son, dest_node->r_son);
        dest_node->r_son->parent = dest_node;
    }

#ifdef DEBUG
    dumpExpNode(dest_node);
#endif

    return 0;

}

//TODO add assert or something
Exp_node * createNode(Node_type type, Value value, Exp_node * l_son, Exp_node * r_son)
{
    int error_code = 0;
    PARSE_ERROR(error_code, type == NUL, MATAN_KILLER_ERROR_INVALID_TYPE);
    // PARSE_ERROR(error_code, !value, MATAN_KILLER_ERROR_INVALID_VALUE);

    if (error_code != 0)
        return nullptr;

    Exp_node * new_node = nodeCtor(); 

    new_node->type = type;
    new_node->value = value;
    new_node->l_son = l_son;
    new_node->r_son = r_son;

    return new_node;

}

int linkToParent(Exp_node *parent, Exp_node *orphan)
{
    if (parent == nullptr || orphan == nullptr)
        return -1;

    orphan->parent = parent;
    return 0;

}

int linkSonsToParent(Exp_node *node)
{
    if (node->l_son)
        node->l_son->parent = node;

    if (node->r_son)
        node->r_son->parent = node;

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

    }

    switch(node->type)
    {
        case OP:
            printf("%c", node->value.op_value);
            break;
        case NUM:
            printf("%g", node->value.dbl_value);
            break;
        case VAR:
            printf("%c", node->value.var_value);
            break;
        default:
            printf("Блэт");
            break;

    }
 
    if (node->r_son)
    {
        printIn(node->r_son);

    }

        printf(")");
    return;
}

int nodeDtor(Exp_node *node)
{
    if (!node)
        return 0;

    while(1)
    {
    #ifdef DEBUG
        printf("\n\e[0;32mдолжны были зафришеть - \e[0m");
        dumpExpNode(node);
    #endif

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

        #ifdef DEBUG
            printf("\n\e[0;31mзафришен - \e[0m");
            dumpExpNode(node);
        #endif
        
            free(node);
            return 0;

        }

        nodeDtor(node->l_son);

        nodeDtor(node->r_son);
    }

    return 0;
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