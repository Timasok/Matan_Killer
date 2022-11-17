#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "matan_killer_f.h"
#include "matan_killer_debug.h"

int getExpression(Exp_node **main_node)
{
    char input[MAX_BUFFER_LENGTH] = {};

    fscanf(stdin, "%*[\n]" );
    fprintf(stdin,"\n");
    scanf("%[^\n]s", input);

    printf("look what we have here %s\n", input);

    return 0;
}

int readExpression(Exp_node * exp_node, const char * input, size_t shift, int free_port)
{

    size_t len = strlen(input);

    if (input == nullptr || shift == len)
        return 0;

    const char * open_bracket  = strchr(&input[shift], '(');
    const char * close_bracket = strchr(&input[shift], ')');

    // printf("\nopen_bracket = %s close_bracket = %s", open_bracket, close_bracket);
    if (open_bracket == nullptr && close_bracket == nullptr)
    {
        readExpression(exp_node, input, len, RIGHT_SON);
        return 0;
    }

    const char * first_bracket;

    if (open_bracket == nullptr)
    {
        first_bracket = close_bracket;

    }else if(close_bracket == nullptr)
    {
        first_bracket = open_bracket;

    }else {

        first_bracket = (open_bracket < close_bracket) ? open_bracket : close_bracket;

    }


    size_t parsing_length = first_bracket - &input[shift];

    if (first_bracket > input)
    {
        parseTerminalNode(exp_node, &input[shift], parsing_length);

    } else if (&input[shift] == close_bracket)
    {
        readExpression(exp_node->parent, input, shift + 1, (free_port + 1) % 2);

    } else if (&input[shift] == open_bracket)
    {

        Exp_node * new_node = nodeConnect(exp_node, free_port);
        readExpression(new_node, input, shift + 1, LEFT_SON);

    }

    return 0;
}

int parseTerminalNode(Exp_node *exp_node, const char * parsing_start, size_t parsing_length)
{

    char * terminalValue = strdup(parsing_start);
    terminalValue[parsing_length] = '\0';

    double dbl_value;
    char char_value = ' ';
    
    if (sscanf(terminalValue, "%lf", &dbl_value) == 1)
    {
        exp_node->type = NUM;
        exp_node->dbl_value = dbl_value;
        return 0;
    
    }else if(parsing_length == 1)
    {
        sscanf(terminalValue, "%c", &char_value);

        if (isalpha(char_value))
        {
            exp_node->type = VAR;
            exp_node->dbl_value = char_value;
            return 0;

        }else if(char_value == '+')
        {
            exp_node->type = OP;
            exp_node->op_value = ADD;
            return 0;

        }else if(char_value == '-')
        {
            exp_node->type = OP;
            exp_node->op_value = SUB;
            return 0;

        }else if(char_value == '*')
        {
            exp_node->type = OP;
            exp_node->op_value = MUL;
            return 0;

        }else if(char_value == '\\')
        {
            exp_node->type = OP;
            exp_node->op_value = DIV;
            return 0;
        }

    }

    return MATAN_KILLER_ERROR_CAN_NOT_PARSE_NODE;

}