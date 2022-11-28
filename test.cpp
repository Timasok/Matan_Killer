# 0 "./src/matan_killer_debug.cpp"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "./src/matan_killer_debug.cpp"







const char TEX_LOG_PATH[] = "log.tex";

static FILE * HTM_LOG = 0;
static FILE * GRAPH_LOG = 0;
static FILE * TEX_LOG = 0;

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
            printf("\nVARIABLE (%c)\n", exp_node->value.var_value);
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

    fprintf(TEX_LOG, "\\documentclass{article}\n"
                        "\\usepackage[utf8]{inputenc}\n"
                        "\\usepackage{graphicx}\n"
                        "\\usepackage[14pt]{extsizes}\n"
                        "\\usepackage{amsmath, amsfonts, amssymb, amsthm, mathtools}\n");







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
            sprintf(data, "\nVARIABLE (%c)\n", node->value.var_value);
            break;
        default:
            sprintf(data, "\nБлэт не определился\n");
            break;

    }

    fprintf(GRAPH_LOG, "node%d [\n" "label=<\n" "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n" "    <tr><td bgcolor=\"#dfdf91\" port = \"H%d\">NODE %d = %p PARENT = %p</td></tr>\n" "    <tr><td bgcolor= \"#5ca1cc\"><font color=\"black\"> %s </font></td></tr>\n" "    <tr>\n" "    <td>\n" "        <table border=\"0\" cellborder=\"1\">\n" "        <tr>\n" "            <td bgcolor = \"#70de9f\" port = \"L%d\">  LEFT = %p </td>\n" "            <td bgcolor = \"#e02828\" port = \"R%d\"> RIGHT = %p </td>\n" "        </tr> \n" "        </table> \n" "    </td>\n" "    </tr>\n" "</table>>\n" "]\n\n", nodeNumber, nodeNumber, nodeNumber, node, node->parent, data, nodeNumber, node->l_son, nodeNumber, node->r_son)
# 175 "./src/matan_killer_debug.cpp"
                                                                                                                                        ;





    if (prevNumber != 0)
        fprintf(GRAPH_LOG, "node%d -> node%d:H%d;", prevNumber, nodeNumber, nodeNumber);


    return 0;
}

int makeDot(Exp_node *exp_node)
{
    GRAPH_LOG = fopen("graph_log.html", "w");

    fprintf(GRAPH_LOG, "digraph MYG {\n");
    fprintf(GRAPH_LOG, "rankdir = VR;\n");
    fprintf(GRAPH_LOG, "graph [splines = ortho];\n");
    fprintf(GRAPH_LOG, "bgcolor = \"white\";\n");

    fprintf(GRAPH_LOG, "node [shape = \"plaintext\", style = \"solid\"];\n");

    findNext(exp_node, *oper);

    fprintf(GRAPH_LOG, "\n}");

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

    fprintf(TEX_LOG, "\\maketitle\n"
                      "\\section{Introduction}\n");
# 232 "./src/matan_killer_debug.cpp"
 fprintf(TEX_LOG, "Due to the heavy workload at the Department of Higher Cringe, "
                    "the author of this article has repeatedly postponed the publication "
                    "of this work. But once the heel pressed, the notebook with differentiation "
                    "formulas fell out of the briefcase in front of the astonished students in the audience, "
                    "into which they each burst with such enthusiasm in the hope of taking a "
                    "bite out of the great knowledge of matan. After that lecture, "
                    "when our editorial team saw the beaming face of Professor Dymarych Petrovich, "
                    "with shaking hands turning over the abstract and whispering something, "
                    "we finally decided to publish those of his drafts that will be understandable to the masses. ");

    fprintf(TEX_LOG, "\\section{Work process}\n");


    return 0;
}

int treeDump(Exp_node *node , const char * operation_info, const char *name_of_file, const char *name_of_func, int number_of_line)
{

    nodeNumber = 0;
    makeDot(node);

    system(dump_comand);

    fprintf(HTM_LOG, "<h2>Dump No. %d\n", pic_number-1);
    fprintf(HTM_LOG, "\t\tDUMP CALLED at file: %s func: %s line: %d\n<h2>", name_of_file, name_of_func, number_of_line);

    if (strlen(operation_info) > 0)
        fprintf(HTM_LOG, "\t\tOPERATION INFO %s<h2>", operation_info);

    fprintf(HTM_LOG, "\n<hr>\n<img src = graph_dumps/dump_%d.jpeg>\n", getDumpNumber());

    fprintf(TEX_LOG, "\n$$\n");

    printInOrderTex(node);

    fprintf(TEX_LOG, "\n$$\n");
    fflush(TEX_LOG);

    return 0;
}

int openPDF()
{

    TEX_LOG = fopen(TEX_LOG_PATH, "a+");

    fprintf(TEX_LOG, "\\end{document}");
    fflush(TEX_LOG);

    fclose(TEX_LOG);

    char command[512] ={};
    sprintf(command, "pdflatex %s > nul 2>&1", TEX_LOG_PATH);

    system(command);

    system("xdg-open log.pdf > nul 2>&1");
    TEX_LOG = fopen(TEX_LOG_PATH, "a+");

    return 0;
}
# 302 "./src/matan_killer_debug.cpp"
int getPriority(const Exp_node *node)
{
    int result = 0;
    Operator re = MUL;

    switch(re)
    {
# 1 "./lib/operations.h" 1
# 13 "./lib/operations.h"
case NOT_OP: { result = 0; break; }

case ADD: { result = 4; break; }

case SUB: { result = 4; break; }

case MUL: { result = 3; break; }

case DIV: { result = 3; break; }

case POW: { result = 2; break; }

case SIN: { result = 1; break; }

case LN: { result = 1; break; }

case COS: { result = 1; break; }

case TAN: { result = 1; break; }

case SH: { result = 1; break; }

case CH: { result = 1; break; }

case TH: { result = 1; break; }

case ARCTG: { result = 1; break; }

case ARCCOS: { result = 1; break; }
# 310 "./src/matan_killer_debug.cpp" 2

    }

    return result;
}
# 325 "./src/matan_killer_debug.cpp"
static int getTexTranscriptForOperation(Operator operation, char * result)
{
    ASSERT(result != NULL);

    Operator re = MUL;

    switch(re)
    {
# 1 "./lib/operations.h" 1
# 13 "./lib/operations.h"
case NOT_OP: { strncpy(result, "", MAX_OPER_LEN); break; }

case ADD: { strncpy(result, "+", MAX_OPER_LEN); break; }

case SUB: { strncpy(result, "-", MAX_OPER_LEN); break; }

case MUL: { strncpy(result, "\\cdot", MAX_OPER_LEN); break; }

case DIV: { strncpy(result, "", MAX_OPER_LEN); break; }

case POW: { strncpy(result, "^", MAX_OPER_LEN); break; }

case SIN: { strncpy(result, "sin", MAX_OPER_LEN); break; }

case LN: { strncpy(result, "ln", MAX_OPER_LEN); break; }

case COS: { strncpy(result, "cos", MAX_OPER_LEN); break; }

case TAN: { strncpy(result, "tan", MAX_OPER_LEN); break; }

case SH: { strncpy(result, "sh", MAX_OPER_LEN); break; }

case CH: { strncpy(result, "ch", MAX_OPER_LEN); break; }

case TH: { strncpy(result, "th", MAX_OPER_LEN); break; }

case ARCTG: { strncpy(result, "arctg", MAX_OPER_LEN); break; }

case ARCCOS: { strncpy(result, "arccos", MAX_OPER_LEN); break; }
# 334 "./src/matan_killer_debug.cpp" 2
    }

    return 0;
}



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
        fprintf(TEX_LOG, "%c", node->value.var_value);

    } else if (node->type == OP)
    {
        int priority = getPriority(node);
# 374 "./src/matan_killer_debug.cpp"
        bool need_additional_brackets = false;

        dumpExpNode(node);

        if (node->parent != nullptr)
        {
            bool need_additional_brackets = ((getPriority(node) > getPriority(node->parent)) && priority > 1);
            printf("priority = %d parent_priority = %d %d\n", getPriority(node), getPriority(node->parent), need_additional_brackets);

        } else
        {
            printf("priority = %d %d\n", getPriority(node), need_additional_brackets);
        }


        if (need_additional_brackets)
        {
            fprintf(TEX_LOG, "(");
        }

        if (priority == UNAR_OP)
        {
            fprintf(TEX_LOG, "%c", node->value.op_value);

            fprintf(TEX_LOG, "(");

            printInOrderTex(node->l_son);

            fprintf(TEX_LOG, ")");

        }else {

            if (node->l_son && priority )
            {
                printInOrderTex(node->l_son);
            }
            if (node->r_son)
            {
                printInOrderTex(node->r_son);
            }

            if (need_additional_brackets)
            {
                fprintf(TEX_LOG, ")");
            }
        }

    }

    fflush(TEX_LOG);

    return 0;

}
