#include <stdio.h>
#include <stdlib.h>

#include "matan_killer_f.h"
#include "matan_killer_debug.h"

static FILE * log = 0;
static FILE *graph_log = 0;
static int pic_number = 0;
static char dump_comand[128] = {};

static int nodeNumber = 0;
static int prevNumber = 0;

int dumpExpNode(Exp_node * exp_node)
{

    switch(exp_node->type)
    {
        case OP:
            printf("\nOPERATION (%c)\n", exp_node->op_value);
            break;
        case NUM:
            printf("\nNUMBER (%g)\n", exp_node->dbl_value);
            break;
        case VAR:
            printf("\nVARIABLE (%c)\n", exp_node->var_value);
            break;
        default:
            printf("\nБлэт не определился\n");
            break;

    }
    
    printf("\tparent %p\n", exp_node->parent);
    printf("\texp_node %p\n", exp_node);
    printf( "l_son %p, r_son %p\n", exp_node->l_son, exp_node->r_son);
    return 0;
}

int openLogs()
{
    log = fopen("log.htm", "w+");
    fflush(log);
    fprintf(log, "<pre>");

    return 1;

}

int closeLogs()
{
   fprintf(log, "\n</pre>");
   fclose(log);

   return 1;
}

int getDumpNumber()
{
    return pic_number - 1;
}

int findNext(const Exp_node* node, int (*oper)(const Exp_node *))
{
    if (!node)
        return 0;

    int currentNumber = ++nodeNumber;    
    oper(node);

    if (node->l_son)
    {
        prevNumber = nodeNumber;
        findNext(node->l_son, *oper);

    }

    if (node->r_son)
    {
        prevNumber = currentNumber;
        findNext(node->r_son, *oper);

    }

    return 0;

}

#define PRINT_DOT(...) fprintf(graph_log, __VA_ARGS__)

static int oper(const Exp_node * node)
{

    char data[MAX_BUFFER_LENGTH] = {};

    switch(node->type)
    {
        case OP:
            sprintf(data, "\nOPERATION (%c)\n", node->op_value);
            break;
        case NUM:
            sprintf(data, "\nNUMBER (%g)\n", node->dbl_value);
            break;
        case VAR:
            sprintf(data, "\nVARIABLE (%c)\n", node->var_value);
            break;
        default:
            sprintf(data, "\nБлэт не определился\n");
            break;

    }

    PRINT_DOT(  "node%d [\n" "label=<\n"
                "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n"
                "    <tr><td bgcolor=\"#dfdf91\" port = \"H%d\">NODE %d = %p</td></tr>\n"
                "    <tr><td bgcolor= \"#5ca1cc\"><font color=\"black\"> %s </font></td></tr>\n"
                
                "    <tr>\n"
                "    <td>\n"
                
                "        <table border=\"0\" cellborder=\"1\">\n"
                
                "        <tr>\n"
                
                "            <td bgcolor = \"#70de9f\" port = \"L%d\">  LEFT = %p </td>\n"
                
                "            <td bgcolor = \"#e02828\" port = \"R%d\"> RIGHT = %p </td>\n"
                
                "        </tr> \n"
                
                "        </table> \n"
                
                "    </td>\n"
                
                "    </tr>\n" 
                
                "</table>>\n"
                "]\n\n", nodeNumber, nodeNumber, nodeNumber, node, data, nodeNumber, node->l_son, nodeNumber, node->r_son);
    
    //There is no use yet to decide which port from the parent we should connect to his son GRAPHVIZ does it! 
    // const char output_port = 0;
    // if ()

    if (prevNumber != 0)
        PRINT_DOT("node%d -> node%d:H%d;", prevNumber, nodeNumber, nodeNumber);

    
    return 0;
}

int makeDot(Exp_node *exp_node)
{
    graph_log = fopen("graph_log.html", "w");
    
    PRINT_DOT("digraph MYG {\n");
    PRINT_DOT("rankdir = VR;\n");//or TB
    PRINT_DOT("graph [splines = ortho];\n");
    PRINT_DOT("bgcolor = \"white\";\n");

    PRINT_DOT("node [shape = \"plaintext\", style = \"solid\"];\n");
    
    findNext(exp_node, *oper);
    // oper(exp_node->main_node);

    PRINT_DOT("\n}");

    fclose(graph_log);

    sprintf(dump_comand, "dot -Tjpeg graph_log.html > graph_dumps/dump_%d.jpeg", pic_number++);
    
    return 0;

}

int treeDump(Exp_node * exp_node)
{
    nodeNumber = 0;
    makeDot(exp_node);
    system(dump_comand);

    return 0;
}

#undef PRINT_DOT