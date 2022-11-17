#ifndef AK_DEBUG_H
#define AK_DEBUG_H

#include "akinator_f.h"

#define TREE_DUMP(treePtr)                                                  \
        treeDump(treePtr, __FILE__, __FUNCTION__,__LINE__)

#define PARSE_ERROR(treePtr, condition, error_specifier)                    \
        listPtr->code_of_error  |= ((condition) & error_specifier);

#define PRINT_ERROR(error_specifier)                                        \
        fprintf(log, "%s", #error_specifier);

int verifyTree(Tree_t *tree);

int openLogs();
int closeLogs();
int getDumpNumber();
int nodeDump(const Node * node);

int nodeAssert(Node * node, const char * value);
int findNext(const Node* node, int (*oper)(const Node *));
int makeDot(Tree_t *tree);
int treeDump(Tree_t *tree, const char *name_of_file, const char *name_of_func, int number_of_line);

#define DEF_ERR(name, num) \
            AK_ERROR_##name = 1 << num,

enum AK_ERRORS
{
        #include "errors.h"
};

#undef DEF_ERR
#endif