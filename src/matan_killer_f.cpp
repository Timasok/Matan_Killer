#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "matan_killer_f.h"
#include "matan_killer_debug.h"
#include "dsl.h"

static Exp_node * createNum(int number)
{
    Value val = {};
    val.dbl_value = number;
    return createNode(NUM, val, nullptr, nullptr);
}

static Exp_node * createOp(Operator op)
{
    Value val = {};
    val.op_value = op;
    return createNode(OP, val, nullptr, nullptr);
}

int getExpression(Text_info *text, Exp_node *main_node)
{
    if (text->buf == nullptr)
    {

        char input[MAX_BUFFER_LENGTH] = {};

        fscanf(stdin, "%*[\n]" );
        fprintf(stdin,"\n");
        scanf("%[^\n]s", input);

        readExpression(main_node, input, 0, LEFT_SON);
        
    }else{

        readExpression(main_node, text->lines[0], 0, LEFT_SON);
    }
    
    return 0;
}

int readExpression(Exp_node * exp_node, const char * input, size_t shift, int free_port)
{

    size_t len = strlen(input);

    if (input == nullptr || shift == len)
        return 0;

#ifdef DEBUG
    printf("\ninput = %s\n", &input[shift]);
#endif

    const char * open_bracket  = strchr(&input[shift], '(');
    const char * close_bracket = strchr(&input[shift], ')');

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

#ifdef DEBUG
    printf("open_bracket = %s close_bracket = %s first_bracket = %s\n", open_bracket, close_bracket, first_bracket);
#endif

    size_t parsing_length = first_bracket - &input[shift];

#ifdef DEBUG
    printf("parsing_length = %lu free_port = %d\n", parsing_length, free_port);
#endif

    if (parsing_length > 0)
    {
        parseTerminalNode(exp_node, &input[shift], parsing_length);
    
    #ifdef DEBUG
        dumpExpNode(exp_node);
    #endif

        readExpression(exp_node, input, shift + parsing_length, free_port);

    } else if (&input[shift] == close_bracket)
    {
        readExpression(exp_node->parent, input, shift + 1, (free_port + 1) % 2);

    } else if (&input[shift] == open_bracket)
    {

        Exp_node * new_node = nodeConnect(exp_node, free_port);
        readExpression(new_node, input, shift + 1, free_port    );

    }else{
    
        readExpression(exp_node, input, shift + 1, free_port);
    }

    return 0;
}

int parseTerminalNode(Exp_node *exp_node, const char * parsing_start, size_t parsing_length)
{

    char * terminalValue = strdup(parsing_start);
    terminalValue[parsing_length] = '\0';

#ifdef DEBUG
    printf("input_segment =%s \nparsed_segment = %s\n", parsing_start, terminalValue);
#endif

    double value;
    char char_value = ' ';
    
    if (sscanf(terminalValue, "%lf", &value) == 1)
    {
        exp_node->type = NUM;
        exp_node->value.dbl_value = value;
        free(terminalValue);
        return 0;
    
    }else /*if(parsing_length == 1)*///TODO maybe add check in here
    {
        sscanf(terminalValue, " %c", &char_value);
        free(terminalValue);

        if (isalpha(char_value))
        {
            exp_node->type = VAR;
            exp_node->value.var_value = char_value;
            return 0;
//TODO make more compact
        }else if(char_value == '+')
        {
            exp_node->type = OP;
            exp_node->value.op_value = ADD;
            return 0;

        }else if(char_value == '-')
        {
            exp_node->type = OP;
            exp_node->value.op_value = SUB;
            return 0;

        }else if(char_value == '*')
        {
            exp_node->type = OP;
            exp_node->value.op_value = MUL;
            return 0;

        }else if(char_value == '\\')
        {
            exp_node->type = OP;
            exp_node->value.op_value = DIV;
            return 0;
        
        }else if(char_value == '^')
        {
            exp_node->type = OP;
            exp_node->value.op_value = POW;
            return 0;
        }

    }

    return MATAN_KILLER_ERROR_CAN_NOT_PARSE_NODE;

}

Exp_node * differentiate(const Exp_node *node)
{
    switch (node->type)
    {
        case NUM:
            return createNum(0);
            break;
        case VAR:
            return createNum(1);
            break;
        case OP:
            Exp_node * new_node = nodeCtor();
            switch(node->value.op_value)
            {
                case ADD:
                case SUB:
                    diffNode(node, new_node, LEFT_SON);
                    diffNode(node, new_node, RIGHT_SON);
                    copyNodeData(node, new_node);
                    return new_node;
                    break;
                
                case MUL:
                
                    makeOp(ADD);
                    
                    leftOp(MUL);
                    dL(LEFT_SON);
                    cL(RIGHT_SON);

                    rightOp(MUL);
                    dR(RIGHT_SON);
                    cR(LEFT_SON);

                    return new_node;
                    break;

                //without dsl
                    // new_node->type = OP;
                    // new_node->value.op_value = ADD;

                    // new_node->l_son = createOp(MUL);
                    // linkToParent(new_node, new_node->l_son);

                    // diffNode(node, new_node->l_son, LEFT_SON);
                    // copyNode(node, new_node->l_son, RIGHT_SON);

                    // new_node->r_son = createOp(MUL);
                    // linkToParent(new_node, new_node->r_son);                  

                    // diffNode(node, new_node->r_son, RIGHT_SON);
                    // copyNode(node, new_node->r_son, LEFT_SON);
                    // return new_node;
                    // break;

                case POW:
                case DIV:
                    break;

                default:
                    printf("go f smbdy");
                    break;
            }
            break;
    }

    return nullptr;
}

int diffNode(const Exp_node *argument, Exp_node * result, const char dest)
{
    switch (dest)
    {
        case LEFT_SON:
            result->l_son = differentiate(argument->l_son);
            result->l_son->parent = result;
            break;
        case RIGHT_SON:
            result->r_son = differentiate(argument->r_son);
            result->r_son->parent = result;
            break;
        default:
            return MATAN_KILLER_ERROR_INCORRECT_DESTINATION_PORT;
            break;
    }

    return 0;
}

int copyNode(const Exp_node *argument, Exp_node * result, const char dest)
{
    switch (dest)
    {
        case LEFT_SON:
            result->l_son = copy(argument->l_son);
            result->l_son->parent = result;
            break;
        case RIGHT_SON:
            result->r_son = copy(argument->r_son);
            result->r_son->parent = result;
            break;
        default:
            return MATAN_KILLER_ERROR_INCORRECT_DESTINATION_PORT;
            break;
    }

    return 0;

}