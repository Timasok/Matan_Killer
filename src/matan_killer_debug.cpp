#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matan_killer_f.h"
#include "matan_killer_debug.h"

const char TEX_LOG_PATH[] = "log.tex";

static FILE * HTM_LOG = 0;
static FILE * GRAPH_LOG = 0;
static FILE * TEX_LOG = 0;

char comments[10][MAX_BUFFER_LENGTH] = {"It is not difficult to follow the simplification", 
                                      "Let's make all the necessary calculations",
                                      "It's obvious that",
                                      "You can check the calculations yourself",
                                      "There should be no doubt about it",
                                      "Readers can take this as an exercise",
                                      "If it seems to you that the transitions are not strict, then cross yourself",
                                      "Do not forget to drink to matan before going to bed",
                                      "We understand that this article is not able to convey the full power of mathematics",
                                      "Landau does not remember when he did not know how to differentiate, you will surely learn!"};

static int pic_number = 0;
static char dump_comand[128] = {};

static int nodeNumber = 0;
static int prevNumber = 0;

int dumpExpNode(const Exp_node * exp_node)
{
    if (!exp_node)
        return -1;

    switch(exp_node->type)
    {
        case OP:
            printf("\nOPERATION (%c)\n", exp_node->value.op_value);
            break;
        case NUM:
            printf("\nNUMBER (%g)\n", exp_node->value.dbl_value);
            break;
        case VAR:
            printf("\nVARIABLE (%s)\n", exp_node->value.var.name);
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
    TEX_LOG = fopen(TEX_LOG_PATH, "w+");

    if (TEX_LOG == NULL)
    {
        printf("ERROR! CANNOT OPEN TEX_FILE!");
        return -1;
    }

    fprintf(TEX_LOG,    "\\documentclass{article}\n"
                        /*"\\documentclass[9pt]{extarticle}\n"*/
                        "\\usepackage[utf8]{inputenc}\n"
                        "\\usepackage{graphicx}\n"
                        "\\usepackage[14pt]{extsizes}\n"
                        
                        "\\usepackage{amsmath, amsfonts, amssymb, amsthm, mathtools}\n");
                        
    // fprintf(TEX_LOG, "\n\\title{\\textbf{Работа по взятию производной у функции}}\n"
    //                  "\\author{Timasok aka Профессор вычислительного цеха "
    //                  "aka Начальник бригады по посадке деревьев выражений}\n"
    //                  "\\data{\\today}\n\n"
    //                  "\\begin{document}}\n");
    
    fprintf(TEX_LOG, "\n\\title{\\textbf{Taking the derivative of a function}}\n"
                     "\\author{Timasok aka Boss of the Tree Planting Team}\n\n"
                     "\\begin{document}\n");
    
    printIntroInTex();

    fflush(TEX_LOG);

    HTM_LOG = fopen("log.htm", "w+");
    fflush(HTM_LOG);
    fprintf(HTM_LOG, "<pre>");

    return 0;

}

int closeLogs()
{
   fprintf(HTM_LOG, "\n</pre>");
   fclose(HTM_LOG);
   fclose(TEX_LOG);

   return 0;
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

#define PRINT_DOT(...) fprintf(GRAPH_LOG, __VA_ARGS__)

static int oper(const Exp_node * node)
{

    char data[MAX_BUFFER_LENGTH] = {};

    switch(node->type)
    {
        case OP:
            sprintf(data, "\nOPERATION (%c)\n", node->value.op_value);
            break;
        case NUM:
            sprintf(data, "\nNUMBER (%g)\n", node->value.dbl_value);
            break;
        case VAR:
            sprintf(data, "\nVARIABLE (%s)\n", node->value.var.name);
            break;
        default:
            sprintf(data, "\nБлэт не определился\n");
            break;

    }

    PRINT_DOT(  "node%d [\n" "label=<\n"
                "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n"
                "    <tr><td bgcolor=\"#dfdf91\" port = \"H%d\">NODE %d = %p PARENT = %p</td></tr>\n"
                "    <tr><td bgcolor= \"#5ca1cc\"><font color=\"black\"> %s </font></td></tr>\n"
                
                "    <tr>\n"
                "    <td>\n"
                
                "        <table border=\"0\" cellborder=\"1\">\n"
                
                "        <tr>\n"
                
                "            <td bgcolor = \"#70de9f\" port = \"P%d\">  LEFT = %p </td>\n"
                
                "            <td bgcolor = \"#e02828\" port = \"P%d\"> RIGHT = %p </td>\n"
                
                "        </tr> \n"
                
                "        </table> \n"
                
                "    </td>\n"
                
                "    </tr>\n" 
                
                "</table>>\n"
                "]\n\n", nodeNumber, nodeNumber, nodeNumber, node, node->parent, data, nodeNumber, node->l_son, nodeNumber, node->r_son);
    
    //There is no use yet to decide which port from the parent we should connect to his son GRAPHVIZ does it! 
    // const char output_port = 0;
    // if ()

    if (prevNumber != 0 && nodeNumber != 0)
        PRINT_DOT("node%d:P%d -> node%d:H%d;", prevNumber, prevNumber, nodeNumber, nodeNumber);

    
    return 0;
}

int makeDot(Exp_node *exp_node)
{
    GRAPH_LOG = fopen("graph_log.html", "w");
    
    PRINT_DOT("digraph MYG {\n");
    PRINT_DOT("rankdir = VR;\n");//or TB
    PRINT_DOT("graph [splines = ortho];\n");
    PRINT_DOT("bgcolor = \"white\";\n");

    PRINT_DOT("node [shape = \"plaintext\", style = \"solid\"];\n");
    
    findNext(exp_node, *oper);

    PRINT_DOT("\n}");

    fclose(GRAPH_LOG);

    sprintf(dump_comand, "dot -Tjpeg graph_log.html > graph_dumps/dump_%d.jpeg", pic_number++);
    
    return 0;

}

int printIntroInTex()
{
    if (TEX_LOG == NULL)
    {
        printf("ERROR! CANNOT OPEN TEX_FILE!");
        return -1;
    }

    fprintf(TEX_LOG,  "\\maketitle\n"
                      "\\section{Introduction}\n");

    // fprintf(TEX_LOG,  "\\textsl{В связи с тяжелейшей загруженностью на кафедре ВышКринжа автор этой статьи "
    //                   "неоднократно переносил публикацию данной работы. Но раз каблук надавил "
    //                   "блокнот с формулами дифференцирования вывалился из портфеля на глазах \n"
    //                   "у изумлённых студентов в аудитории, в которую они каждый с таким энтузиазмом"
    //                   "врываются в надежде откусить кусочек от великого знания матана. "
    //                   "После той лекции как наш редакторский коллектив увидел сияющее лицо Профессора Дымарыча, "
    //                   "трясущимися руками переворачивающего конспекта и что-то нашоптывающего \n"
    //                   "мы наконец решились опубликовать, те из его черновиков "
    //                   "которые будут понятны массам.}\n");    

 fprintf(TEX_LOG,   "Due to the heavy workload at the Department of Higher Cringe, " 
                    "the author of this article has repeatedly postponed the publication "
                    "of this work. But once the heel pressed,the notebook with differentiation "
                    "formulas fell out of the briefcase in front of the astonished students in the audience, "
                    "into which they each burst with such enthusiasm in the hope of taking a "
                    "bite out of the great knowledge of matan. After that lecture, "
                    "when our editorial team saw the beaming face of Professor Dymarych Petrovich, "
                    "with shaking hands turning over the abstract and whispering something, "
                    "we finally decided to publish those of his drafts that will be understandable to the masses. ");

    fprintf(TEX_LOG,  "\n\\section{Work process}\n");


    return 0;
}

int treeDump(Exp_node *node , const char * operation_info, const char *name_of_file, const char *name_of_func, int number_of_line)
{
    // graphDumpDot(list);
    nodeNumber = 0;
    makeDot(node);

    system(dump_comand);

    fprintf(HTM_LOG, "<h2>Dump No. %d\n", pic_number-1);
    fprintf(HTM_LOG, "\t\tDUMP CALLED at file: %s func: %s line: %d\n<h2>", name_of_file, name_of_func, number_of_line);
     
    if (strlen(operation_info) > 0)
    {
        fprintf(HTM_LOG, "\n\t\tOPERATION INFO %s<h2>", operation_info);
        fprintf(TEX_LOG, "\\textbf{%s:}\n", operation_info);    
    }

    fprintf(HTM_LOG, "\n<hr>\n<img src = graph_dumps/dump_%d.jpeg>\n", getDumpNumber());

    fprintf(TEX_LOG, "\n$$\n");

    printInOrderTex(node);
    
    fprintf(TEX_LOG, "\n$$\n");
    fflush(TEX_LOG);

    return 0;
}

static char * getCringeComment()
{
    return comments[rand()%10];

}

int saveMicroTransform(const Exp_node *node)
{

    fprintf(TEX_LOG, "%s\n", getCringeComment());    

    fprintf(TEX_LOG, "$$\n");

    printInOrderTex(node);
    
    fprintf(TEX_LOG, "\n$$\n");

    return 0;
}

int openPDF()
{   

    TEX_LOG = fopen(TEX_LOG_PATH, "a+");

    fprintf(TEX_LOG,  "\\end{document}");
    fflush(TEX_LOG);

    fclose(TEX_LOG);

    char command[512] ={};

    sprintf(command, "pdflatex %s > nul 2>&1", TEX_LOG_PATH);
    // sprintf(command, "pdflatex %s", TEX_LOG_PATH);

    system(command);
        // DBG_OUT;
    system("xdg-open log.pdf > nul 2>&1");
    TEX_LOG = fopen(TEX_LOG_PATH, "a+");

    return 0;
}

#define DEF_OP(op_name, priority, op_code, num, oper, str_for_tex)               \
        else if(node->value.op_value == op_name)                                 \
        {                                                                        \
            return priority;                                                     \
        }                                                                        \

int getPriority(const Exp_node *node)
{

    if (0)
    {}
        #include "operations.h"

    return 0;
}

#undef DEF_OP

#define DEF_OP(op_name, priority, op_code, num, oper, str_for_tex)               \
        else if(operation == op_name)                                            \
        {                                                                        \
            strncpy(result, str_for_tex, MAX_OPER_LEN);                          \
            return 0;                                                            \
        }                                                                        \

static int getTexTranscriptForOp(Operator operation, char * result)
{
    ASSERT(result != NULL);

    if (0)
    {}
        #include "operations.h"

    return 0;
}

#undef DEF_OP

int printInOrderTex(const Exp_node *node)
{
    if (!node)
        return 0; 

    if (node->type == NUM)
    {
        bool add_brackets = (node->value.dbl_value < 0);

        if (add_brackets)
            fprintf(TEX_LOG, "(");    

        fprintf(TEX_LOG, "%g", node->value.dbl_value);

        if (add_brackets)
            fprintf(TEX_LOG, ")");  


    } else if (node->type == VAR)
    {
        fprintf(TEX_LOG, "%s", node->value.var.name);

    } else if (node->type == OP)
    {
        int priority = getPriority(node);

        if (priority == 0)
        {
            printf("ATTEMPT TO PRINT NOT OPER!\n");
            return -1;
        }

        bool need_additional_brackets = node->parent && (getPriority(node) > getPriority(node->parent)) && priority > 1;
        Operator op = node->value.op_value;


#ifdef DEBUG
        if (node->parent)
        {
            dumpExpNode(node);
            printf("priority = %d parent_priority = %d %d\n", getPriority(node), getPriority(node->parent), need_additional_brackets);

        } else 
        {
            dumpExpNode(node);
            printf("priority = %d %d\n", getPriority(node), need_additional_brackets);
        }
#endif

        if (priority == UNAR_OP)
        {
            char func_name[MAX_BUFFER_LENGTH] = {};
            getTexTranscriptForOp(op, func_name);
            fprintf(TEX_LOG, "%s", func_name);
            
            fprintf(TEX_LOG, "{(");

            printInOrderTex(node->r_son);

            fprintf(TEX_LOG, ")}");
        
        } else
        {

            if (need_additional_brackets == true)
            {
                fprintf(TEX_LOG, "(");
            }

            if (op == DIV)
            {
                fprintf(TEX_LOG, "\\frac{");
            }

            printInOrderTex(node->l_son);
            
            if (op == DIV)
            {
                fprintf(TEX_LOG, "}{");

            } else
            {
                char func_name[MAX_BUFFER_LENGTH] = {};
                getTexTranscriptForOp(op, func_name);
                fprintf(TEX_LOG, "%s", func_name);
            }
            
            if (op == POW)
                fprintf(TEX_LOG, "{");

            printInOrderTex(node->r_son);

            if (op == DIV || op == POW)
                fprintf(TEX_LOG, "}");

            if (need_additional_brackets == true)
            {
                fprintf(TEX_LOG, ")");
            }
        
        }
    
    }

    fflush(TEX_LOG);

    return 0;

}

#undef PRINT_DOT