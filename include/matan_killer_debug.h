#ifndef MATAN_KILLER_DEBUG_H
#define MATAN_KILLER_DEBUG_H

#define DEF_ERR(name, num) \
            MATAN_KILLER_ERROR_##name = 1 << num,

#define PARSE_ERROR(error_code, condition, error_specifier)                    \
                error_code |= ((condition) & error_specifier);

#define PRINT_ERROR(error_specifier)                                        \
        fprintf(log, "%s", #error_specifier);
        
#define DBG_OUT fprintf(stderr, "file: %s func: %s line: %d\n", __FILE__, __FUNCTION__, __LINE__)

int dumpExpNode(const Exp_node * exp_node);//TODO replace exp_node with node
int getPriority(const Exp_node *node);

int printIntroInTex();
int openPDF();

int saveMicroTransform(const Exp_node *node);

int openLogs();
int closeLogs();
int getDumpNumber();

int findNext(const Exp_node* node, int (*oper)(const Node *));
int makeDot(Exp_node * exp_node);

int treeDump(Exp_node *node, const char * operation_info, const char *name_of_file, const char *name_of_func, int number_of_line);

int printInOrderTex(const Exp_node * node);

#define TREE_DUMP(nodePtr)                                                  \
        treeDump(nodePtr, "", __FILE__, __PRETTY_FUNCTION__,__LINE__)

#define TREE_DUMP_OPTIONAL(nodePtr, operation_info)                                                  \
        treeDump(nodePtr, operation_info, __FILE__, __PRETTY_FUNCTION__,__LINE__)


enum MATAN_KILLER_ERRORS
{
        #include "errors.h"
};

#undef DEF_ERR
#endif