#ifndef MATAN_KILLER_DEBUG_H
#define MATAN_KILLER_DEBUG_H

#define DEF_ERR(name, num) \
            MATAN_KILLER_ERROR_##name = 1 << num,

#define PARSE_ERROR(error_code, condition, error_specifier)                    \
                error_code |= ((condition) & error_specifier);

#define PRINT_ERROR(error_specifier)                                        \
        fprintf(log, "%s", #error_specifier);

int dumpExpNode(Exp_node * exp_node);

int openLogs();
int closeLogs();
int getDumpNumber();

int findNext(const Exp_node* node, int (*oper)(const Node *));
int makeDot(Exp_node * exp_node);
int treeDump(Exp_node * exp_node);

enum MATAN_KILLER_ERRORS
{
        #include "errors.h"
};

#undef DEF_ERR
#endif