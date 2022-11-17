#include <stdio.h>
#include <stdlib.h>

#include "akinator_f.h"
#include "akinator_debug.h"

static FILE * log = 0;
static FILE *graph_log = 0;
static int pic_number = 0;
static char dump_comand[128] = {};

static int nodeNumber = 0;
static int prevNumber = 0;

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

int nodeAssert(Node * node, const char * value)
{
    if (node == nullptr)
    {
        char line_to_say[MAX_BUFFER_LENGTH] = {};
        sprintf(line_to_say, "НЕВОЗМОЖНО НАЙТИ %s В БАЗЕ!!!\n", value);
        sayAndWrite(line_to_say, 'r');
        return -1;
    }else if(!(node->l_son == nullptr && node->r_son == nullptr))
    {
        char line_to_say[MAX_BUFFER_LENGTH] = {};
        sprintf(line_to_say, "%s не объект, а признак!!!\n", value);
        sayAndWrite(line_to_say, 'r');
        return -2;
    }

    return 0;
}

int getDumpNumber()
{
    return pic_number - 1;
}

int findNext(const Node* node, int (*oper)(const Node *))
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

static int oper(const Node * node)
{
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
                "]\n\n", nodeNumber, nodeNumber, nodeNumber, node, node->data, nodeNumber, node->l_son, nodeNumber, node->r_son);
    
    //There is no use yet to decide which port from the parent we should connect to his son GRAPHVIZ does it! 
    // const char output_port = 0;
    // if ()

    if (prevNumber != 0)
        PRINT_DOT("node%d -> node%d:H%d;", prevNumber, nodeNumber, nodeNumber);

    
    return 0;
}

int nodeDump(const Node * node)
{
    printf("\t%s\n", node->data);
    printf("\tparent %p\n", node->parent);
    printf( "l_son %p, r_son %p\n", node->l_son, node->r_son);
    return 0;
}

int makeDot(Tree_t *tree)
{
    graph_log = fopen("graph_log.html", "w");
    
    PRINT_DOT("digraph MYG {\n");
    PRINT_DOT("rankdir = VR;\n");//or TB
    PRINT_DOT("graph [splines = ortho];\n");
    PRINT_DOT("bgcolor = \"white\";\n");

    PRINT_DOT("node [shape = \"plaintext\", style = \"solid\"];\n");
    
    findNext(tree->main_node, *oper);
    // oper(tree->main_node);

     PRINT_DOT("\n}");

    fclose(graph_log);

    sprintf(dump_comand, "dot -Tjpeg graph_log.html > graph_dumps/dump_%d.jpeg", pic_number++);
    
    return 0;

}

#undef PRINT_DOT

int treeDump(Tree_t *tree, const char *name_of_file, const char *name_of_func, int number_of_line)
{
    nodeNumber = 0;
    makeDot(tree);
    system(dump_comand);

    return 0;
}