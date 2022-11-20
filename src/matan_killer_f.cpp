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

#define DEF_OP(op_name, op_code)                                    \
     else if(symbol == op_code)                                     \
    {                                                               \
        result = op_name;                                           \
    }                                                               \

Operator isOp(int symbol)
{
    Operator result = NOT_OP;

    if (0)
    {

    }
    #include "operations.h"    

    return result;
}

#undef DEF_OP

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

        Operator op = isOp(char_value);

        if (op != NOT_OP)
        {
            exp_node->type = OP;
            exp_node->value.op_value = op;
            return 0;

        }else if (isalpha(char_value))
        {
            exp_node->type = VAR;
            exp_node->value.var_value = char_value;
            return 0;

        }

    }

    return MATAN_KILLER_ERROR_CAN_NOT_PARSE_NODE;

}

Exp_node * differentiate(const Exp_node *node)
{
    Exp_node * new_node;

    switch (node->type)
    {
        case NUM:
        {
            new_node = createNum(0);

            break;
        }
        case VAR:
        {
            new_node = createNum(1);

            break;
        }
        case OP:
            new_node = nodeCtor();
            switch(node->value.op_value)
            {
                case ADD:
                case SUB:
                {

                    d(LEFT_SON, LEFT_SON);
                    d(RIGHT_SON, RIGHT_SON);

                    copyOp();

                    break;

                }
                case MUL:
                {
                    makeOp(ADD);
                    
                //first level

                    leftOp(MUL);
                    rightOp(MUL);
                    
                //second level

                    dL(LEFT_SON, LEFT_SON);
                    cL(RIGHT_SON, RIGHT_SON);
                    
                    dR(RIGHT_SON, RIGHT_SON);
                    cR(LEFT_SON, LEFT_SON);

                    break;

                }
                case POW:
                {
                    if (node->r_son->type == NUM)
                    {
                   
                        makeOp(MUL);

                    //creating first level
                        d(LEFT_SON, LEFT_SON);
                        rightOp(MUL);

                    //second level

                        cR(LEFT_SON, RIGHT_SON);
                        right_right_Op(POW);

                    //third level

                        cRR(LEFT_SON, LEFT_SON);
                        right_right_right_Op(SUB);       

                    //fourth level

                        cRRR(LEFT_SON, RIGHT_SON);
                        new_node->r_son->r_son->r_son->r_son = createNum(1);
                        linkSonsToParent(new_node->r_son->r_son->r_son);

                    }else {
                        
                    //     makeOp(MUL);

                    // //creating first level
                    //     leftOp(POW);
                    //     rightOp(ADD);

                    // //second level

                    //     cR(LEFT_SON, LEFT_SON);
                    //     cR(RIGHT_SON, RIGHT_SON);

                    //     right_left_Op(MUL);
                    //     right_right_Op(MUL);
                                       
                    // //third level

                    //     dRL(LEFT_SON, LEFT_SON);
                    //     cRL(RIGHT_SON, RIGHT_SON);
                        
                    //     cRR()

                    }

                    break;
                }
                case DIV:
                {
                    makeOp(DIV);

                //first level

                    leftOp(SUB);
                    rightOp(POW);

                //second level

                    left_left_Op(MUL);
                    left_right_Op(MUL);

                    cR(LEFT_SON,RIGHT_SON);
                    new_node->r_son->r_son = createNum(2);
                    linkSonsToParent(new_node->r_son);

                //third level

                    dLL(LEFT_SON, LEFT_SON);
                    cLL(RIGHT_SON, RIGHT_SON);

                    dLR(LEFT_SON, RIGHT_SON);
                    cLR(RIGHT_SON, LEFT_SON);
                    
                    break;
                }
                case SIN:
                {
                    makeOp(MUL);

                //creating first level
                    d(RIGHT_SON, RIGHT_SON);
                    leftOp(COS);

                //second level

                    new_node->l_son->l_son = createNum(0);
                    linkSonsToParent(new_node->l_son);
                    cL(RIGHT_SON, RIGHT_SON);

                    break;
                }
                case COS:
                {
                //     makeOp(MUL);

                // //creating first level
                //     d(RIGHT_SON, RIGHT_SON);
                //     leftOp(COS);

                // //second level

                //     new_node->l_son->l_son = createNum(0);
                //     linkSonsToParent(new_node->l_son);
                //     cL(RIGHT_SON, RIGHT_SON);

                    break;
                }
                default:
                    printf("go f smbdy");
                    break;
            }
            break;
    }

    #ifdef DEBUG
        dumpExpNode(new_node);
    #endif
    return new_node;
}

int diffNode(const Exp_node *argument, Exp_node * result, const char linking_side_in_copy, const char src_side)
{
    Exp_node *node;

    switch (src_side)
    {
        case LEFT_SON:

            node = differentiate(argument->l_son);         
            break;

        case RIGHT_SON:
            
            node = differentiate(argument->r_son);
            break;

        default:
            return MATAN_KILLER_ERROR_INCORRECT_DESTINATION_PORT;
            break;
            
    }

    switch (linking_side_in_copy)
    {
        case LEFT_SON:
            result->l_son = node;       
            break;

        case RIGHT_SON:
            
            result->r_son = node;
            break;

        default:
            return MATAN_KILLER_ERROR_INCORRECT_DESTINATION_PORT;
            break;
            
    }

    linkSonsToParent(result);

    return 0;
}

int copyNode(const Exp_node *argument, Exp_node * result, const char linking_side_in_copy, const char src_side)
{
    Exp_node *node;

    switch (src_side)
    {
        case LEFT_SON:

            node = copy(argument->l_son);         
            break;

        case RIGHT_SON:
            
            node = copy(argument->r_son);
            break;

        default:
            return MATAN_KILLER_ERROR_INCORRECT_DESTINATION_PORT;
            break;
            
    }

    switch (linking_side_in_copy)
    {
        case LEFT_SON:
            result->l_son = node;       
            break;

        case RIGHT_SON:
            
            result->r_son = node;
            break;

        default:
            return MATAN_KILLER_ERROR_INCORRECT_DESTINATION_PORT;
            break;
            
    }

    linkSonsToParent(result);

    return 0;
}