#ifndef MATAN_KILLER_H
#define MATAN_KILLER_H

#include "tree.h"
#include "text_funcs.h"

Operator isOp(int symbol);

int getExpression(Text_info *text, Exp_node *main_node);

int readExpression(Exp_node *exp_node, const char * remainder, size_t shift, int free_port);

int parseTerminalNode(Exp_node *exp_node, const char * parsing_start, size_t parsing_length);

Exp_node * differentiate(const Exp_node *exp_node);

int diffNode(const Exp_node *argument, Exp_node * result, const char linking_side_in_copy, const char src_side);
int copyNode(const Exp_node *argument, Exp_node * result, const char linking_side_in_copy, const char src_side);

#endif