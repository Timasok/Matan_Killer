#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#include "matan_killer_f.h"
#include "matan_killer_debug.h"
#include "dsl.h"

const double EPS = 0.00001;

static bool equals(const double num_1, const double num_2)
{
    if (abs(num_1 - num_2) < EPS)
        return true;

    return false;

}

Exp_node * createNum(double number)
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

#define DEF_OP(op_name, op_code, num, oper)                         \
            else if(counter == num)                                 \
            {                                                       \
                original[counter].initial = strdup(#op_name);       \
                sprintf(original[counter].parsed, "(0)%c", op_code);\
                counter++;                                          \
                continue;                                           \
            }                                                       \

Lex_sub * getLexicalSubstitusions()
{
    Lex_sub * original = (Lex_sub *)calloc(MAX_FUNC_NUMBER, sizeof(Lex_sub));

    size_t counter = 0;

    while (counter < MAX_FUNC_NUMBER)
    {
        if (0){}
        #include "operations.h"
        break;
    }

#ifdef DEBUG
    for (int idx = 0; idx < MAX_FUNC_NUMBER; idx++)
    {
        printf("LEX[%d] = {%s, %s}\n", idx, original[idx].initial, original[idx].parsed);

    }
#endif

    return original;

}

int LexDtor(Lex_sub *lex)
{
    if(!lex)
        return -1;

    for (int idx = 0; idx < MAX_FUNC_NUMBER; idx++)
    {
        free(lex[idx].initial);
    }

    free(lex);

    return 0;
}

#undef DEF_OP

char * replaceFuncNames(const char * input)
{

    Lex_sub * lex = getLexicalSubstitusions();

    char * result = nullptr;
    const char * searched = nullptr;

    for (int idx = 0; idx < MAX_FUNC_NUMBER; idx++)
    {

        if (lex[idx].initial == nullptr)
            break;


        size_t initial_len = strlen(lex[idx].initial);

        searched = strstr(input, lex[idx].initial);

        if (searched == nullptr && lex[idx].initial != NULL)
        {
            char * initial_copy = strdup(lex[idx].initial);

            for (int counter = 0; counter < initial_len; counter++)
            {
                initial_copy[counter] = tolower(lex[idx].initial[counter]);
            }

            searched = strstr(input, initial_copy);
            free(initial_copy);
        }

        if (searched != nullptr)
        {
            size_t parsed_len = strlen(lex[idx].parsed);
            size_t input_len  = strlen(input);

            size_t initial_shift = searched - input;
            size_t remainder_len = input_len - initial_shift - initial_len; 

#ifdef DEBUG
        printf("initial_shift = %d parsed_len = %d remainder_len = %d\n", initial_shift, parsed_len, remainder_len);
        printf("initial_len = %d input_len = %d\n\n", initial_len, input_len);
#endif

            result = (char *)calloc(initial_shift + parsed_len + remainder_len + 1, sizeof(char));

#ifdef DEBUG
            printf("allocated space = %d\n", initial_shift + parsed_len + remainder_len);

            STRING_DUMP(input);
            STRING_DUMP(lex[idx].parsed);
            STRING_DUMP(searched + initial_len);
#endif
            strncpy(result, input, initial_shift);

#ifdef DEBUG
            STRING_DUMP(result);
#endif
            strncat(result, lex[idx].parsed, parsed_len);

#ifdef DEBUG            
            STRING_DUMP(result);
#endif            
            strncat(result, searched + initial_len, remainder_len);

#ifdef DEBUG
            STRING_DUMP(result);
#endif
            searched = nullptr;
        }

    }
    
    LexDtor(lex);

    return result;
    
}

int getExpression(Text_info *text, Exp_node *main_node)
{
    if (text->buf == nullptr)
    {
        //статический массив передаю блэт
        char input[MAX_BUFFER_LENGTH] = {};

        fscanf(stdin, "%*[\n]" );
        fprintf(stdin,"\n");
        scanf("%[^\n]s", input);

        readExpression(main_node, input, 0, LEFT_SON);
        
    }else{

        char * result = replaceFuncNames(text->lines[0]);

        if (!result)
        {
            readExpression(main_node, text->lines[0], 0, LEFT_SON);

        }else{
        
            readExpression(main_node, result, 0, LEFT_SON);
        }

        free(result);
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
        readExpression(new_node, input, shift + 1, free_port);//TODO спорный мяч

    }else{
    
        readExpression(exp_node, input, shift + 1, free_port);
    }

    return 0;
}

#define DEF_OP(op_name, op_code, num, oper)                         \
     else if(symbol == op_code)                                     \
    {                                                               \
        result = op_name;                                           \
    }                                                               \

Operator isOp(int symbol)
{
    Operator result = NOT_OP;

    if (0)
    { }
    #include "operations.h"    

    return result;
}

static bool isEitherZeroOrOne(Exp_node *node)
{
    if (equalsZero(node) != HOLLOW || equalsOne(node) != HOLLOW)
    {
        return true;
    }else
    {
        return false;
    }
    
}

son equalsZero(Exp_node *node)
{
    if (node->l_son->type == NUM && equals(node->l_son->value.dbl_value,0))
        return LEFT_SON;

    if (node->r_son->type == NUM && equals(node->r_son->value.dbl_value, 0))
        return RIGHT_SON;

    return HOLLOW;
}

son equalsOne(Exp_node *node)
{
    if (node->l_son->type == NUM && equals(node->l_son->value.dbl_value, 1))
        return LEFT_SON;

    if (node->r_son->type == NUM && equals(node->r_son->value.dbl_value, 1))
        return RIGHT_SON;

    return HOLLOW;
}

#undef DEF_OP

bool isTerminal(Exp_node *node)
{
    if (!node)
        return false;

    if (!node->l_son && !node->r_son)
    {
        return true;

    }else {
        
        return false;
    }

}

bool hasSons(Exp_node *node)
{
    //todo add soft assert
    assert(!isTerminal(node));

    if (node->l_son && node->r_son)
    {
        return true;

    } else {

        return false;
    }
}

bool hasNumSons(Exp_node *node)
{
    // ASSERT(!isTerminal(node));

    if (isTerminal(node))
    {
        return false;

    } else if (node->l_son->type == NUM && node->r_son->type == NUM)
    {
        return true;

    } else {

        return false;
    }
}

//DOESN'T WORK MTHF
Exp_node * simplifyTree(Exp_node *node)
{

    Exp_node * old_node = nullptr;
    Exp_node * copy_node = copy(node);

    do {    
        
        old_node = copy_node;
        wrapEquivalents(copy_node);    
        
        computeConstants(copy_node);

    } while (copy_node != old_node);

    nodeDtor(node);

    return copy_node;
}

Exp_node * computeConstants(Exp_node *node)
{
    Exp_node * result = node;

    if (!node) 
        return result;

    if (node->type == VAR)
    {   
        return result;
    }
   
    if (node->type == OP && hasNumSons(node))
    {
        return rollUpTree(node);
                
    }else {

        // TREE_DUMP_OPTIONAL(node, "not rolled up");
    
    }
        
    if (node->l_son)
    {
        Exp_node *left_result = computeConstants(node->l_son);
        
        if (node->l_son != left_result)
        {
            result->l_son = left_result;
            linkToParent(result, left_result);
        }
    }

    if (node->r_son)
    {
        Exp_node *right_result = computeConstants(node->r_son);
        
        if (node->r_son != right_result)
        {
            result->r_son = right_result;
            linkToParent(result, right_result);
        }
    }

    node = result;
    return result;

}

Exp_node * wrapEquivalents(Exp_node *node)
{
    Exp_node * result = node;
    // dumpExpNode(node);

    if (!node) 
        return result;

    if (node->type == VAR || node->type == NUM)
    {
        return result;
    }

    if (node->type == OP)
    {
    
        if (hasSons(node))
        {

    #ifdef DEBUG
            printf("\nwe should've simplified this node");
            dumpExpNode(node);
    #endif
            Exp_node * res = processOneZeroCases(node);

    #ifdef DEBUG
            printf("old node = %p, new node = %p\n", node, res);
    #endif
            if (res != node)
            {
                // node->parent->l_son = nullptr;
    
    #ifdef DEBUG
                printf("we simplified this node\n");
    #endif            
                // node = res;
                // res->parent = nullptr;
                return res;            
            }
            
        }
            
        if (node->l_son)
        {
            Exp_node *left_result = wrapEquivalents(node->l_son);

            if (result->l_son != left_result)
            {
                result->l_son = left_result;
                linkToParent(result, left_result);
            }
        }

        if (node->r_son)
        {
            Exp_node *right_result = wrapEquivalents(node->r_son);
            
            if (result->r_son != right_result)
            {
                result->r_son = right_result;
                linkToParent(result, right_result);
            }
        }

    }

    node = result;
    return node;

}

#define DEF_OP(op_name, op_code, num, oper)                                                 \
    else if (node->value.op_value == op_code && num == MAGIC_NUMBER_THAT_STANDS_FOR_ARITHM) \
    {                                                                                       \
        val.dbl_value = node->l_son->value.dbl_value oper node->r_son->value.dbl_value;     \
        have_to_roll_up = true;                                                             \
    }                                                                                       \

Exp_node * rollUpTree(Exp_node *node)
{
    bool have_to_roll_up = false;
    Value val = {};
    Exp_node * new_node = node;

    if (0) {}
    #include "operations.h"

    if (have_to_roll_up)
    {
        new_node = createNode(NUM, val, NULL, NULL);
        
        if (node->parent)
        {
            linkToParent(node->parent, new_node);
        }

        nodeDtor(node);

    }

    return new_node;
}

#undef DEF_OP

static bool expressionEqualsConjugate(const int side_of_one, const int side_of_zero, const int conjugate_side, const Operator node_op)
{
    if (node_op == MUL && side_of_one != HOLLOW)
        return true;

    if (node_op == DIV && side_of_one == RIGHT_SON)
        return true;

    if (node_op == ADD && side_of_zero != HOLLOW)
        return true;

    if (node_op == SUB && side_of_zero == RIGHT_SON)
        return true;

    if (node_op == POW && side_of_one == RIGHT_SON)
        return true;

    return false;
}

static bool expressionEqualsZero(const int side_of_one, const int side_of_zero, const int conjugate_side, const Operator node_op)
{
    if (node_op == MUL && side_of_zero != HOLLOW)
        return true;

    if (node_op == POW && side_of_zero == LEFT_SON)
        return true;

    if (node_op == DIV && side_of_zero == LEFT_SON)
        return true;

    return false;
}

static bool expressionEqualsOne(const int side_of_one, const int side_of_zero, const int conjugate_side, const Operator node_op)
{
    if (node_op == POW && side_of_one == LEFT_SON)
        return true;

    if (node_op == POW && side_of_zero == RIGHT_SON)
        return true;

    return false;
}

Exp_node * processOneZeroCases(Exp_node *node)
{
    if (!node)
        return node;

    int side_of_one  = equalsOne(node);
    int side_of_zero = equalsZero(node);

    if (side_of_zero == HOLLOW && side_of_one == HOLLOW)
        return node;
    
    int conjugate_side = HOLLOW;
    
    if (side_of_zero != HOLLOW)
    {
        conjugate_side = (side_of_zero + 1) % 2;

    } else if (side_of_one != HOLLOW)
    {
        
        conjugate_side = (side_of_one + 1) % 2;
    }

#ifdef DEBUG

    dumpExpNode(node);
    printf("\nside_of_one = %d side_of_zero = %d conjugate_side = %d\n", side_of_one, side_of_zero, conjugate_side);
    printf("operation value %c\n", node->value.op_value);
#endif  

    bool node_becomes_num = false;

    Value val = {};
    Exp_node * new_node = node;

    bool eq_exp = expressionEqualsConjugate(side_of_one, side_of_zero, conjugate_side, node->value.op_value);
    bool eq_one = expressionEqualsOne(side_of_one, side_of_zero, conjugate_side, node->value.op_value);
    bool eq_zero = expressionEqualsZero(side_of_one, side_of_zero, conjugate_side, node->value.op_value);

    // printf("eq exp %d, eq one %d, eq zero %d\n", eq_exp, eq_one, eq_one);

    if (eq_exp)
    {

        switch (conjugate_side)
        {
            case LEFT_SON:
            {
                new_node = node->l_son;
                node->l_son = nullptr;
                break;
            }
            case RIGHT_SON:
            {
                new_node = node->r_son;
                node->r_son = nullptr;
                break;            
            }

        }        

        if (node->parent)
        {
            linkToParent(node->parent, new_node);
        }

        nodeDtor(node);

    }else if (eq_one)
    {

        node_becomes_num = true;
        val.dbl_value = 1;

    }else if(eq_zero)
    {

        node_becomes_num = true;
        val.dbl_value = 0;

    }

    if (node_becomes_num)
    {
        new_node = createNode(NUM, val, NULL, NULL);
        
        if (node->parent)
        {
            linkToParent(node->parent, new_node);
        }

        nodeDtor(node);

    }

    return new_node;
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
        {
            new_node = nodeCtor();
            switch(node->value.op_value)
            {
                case ADD:
                case SUB:
                {
                    copyOp();

                    d(LEFT_SON, LEFT_SON);
                    d(RIGHT_SON, RIGHT_SON);

                    // TREE_DUMP_OPTIONAL(new_node, "SUB/ADD DUMP");
                    //TODO add dump for operations
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
                        mRRR(RIGHT_SON, 1);

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
                    mR(RIGHT_SON, 2);

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

                    mL(LEFT_SON, 0);
                    cL(RIGHT_SON, RIGHT_SON);

                    break;
                }
                case COS:
                {
                    makeOp(MUL);

                //first level
                   
                    leftOp(MUL);
                    d(RIGHT_SON, RIGHT_SON);

                //second level
                                   
                    mL(LEFT_SON, -1); 
                    left_right_Op(SIN);  
                
                //third level
                    cLR(RIGHT_SON, RIGHT_SON);
                    mLR(LEFT_SON, 0);

                    break;
                }
                case LN:
                {
                    makeOp(MUL);

                //first level

                    leftOp(DIV);
                    d(RIGHT_SON, RIGHT_SON);

                //second level

                    mL(LEFT_SON, 1);
                    cL(RIGHT_SON, RIGHT_SON);

                    break;
                }
                case TAN:
                {
                    makeOp(DIV);

                    d(LEFT_SON, RIGHT_SON);
                    rightOp(POW);

                    right_left_Op(COS);
                    mR(RIGHT_SON, 2);

                    cRL(RIGHT_SON, RIGHT_SON);
                    mRL(LEFT_SON, 0);

                    TREE_DUMP_OPTIONAL(new_node, "tan");
                    break;
                }
                default:
                    printf("go f smbdy");
                    break;
            }
            break;
        }
    }

    #ifdef DEBUG
        dumpExpNode(new_node);
    #endif

    // printf("before simplifying %p\n", new_node);

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