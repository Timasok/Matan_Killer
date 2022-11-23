# 0 "./src/matan_killer_f.cpp"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "./src/matan_killer_f.cpp"
# 12 "./src/matan_killer_f.cpp"
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
# 35 "./src/matan_killer_f.cpp"
Lex_sub * getLexicalSubstitusions()
{
    Lex_sub * original = (Lex_sub *)calloc(MAX_FUNC_NUMBER, sizeof(Lex_sub));

    size_t counter = 0;

    while (counter < MAX_FUNC_NUMBER)
    {
        if (0){}
# 1 "./lib/operations.h" 1
# 11 "./lib/operations.h"
else if(counter == -2) { original[counter].initial = strdup("NOT_OP"); sprintf(original[counter].parsed, "(0)%c", '\r'); counter++; continue; }

else if(counter == -1) { original[counter].initial = strdup("ADD"); sprintf(original[counter].parsed, "(0)%c", '+'); counter++; continue; }

else if(counter == -1) { original[counter].initial = strdup("SUB"); sprintf(original[counter].parsed, "(0)%c", '-'); counter++; continue; }

else if(counter == -1) { original[counter].initial = strdup("MUL"); sprintf(original[counter].parsed, "(0)%c", '*'); counter++; continue; }

else if(counter == -1) { original[counter].initial = strdup("DIV"); sprintf(original[counter].parsed, "(0)%c", '/'); counter++; continue; }

else if(counter == -3) { original[counter].initial = strdup("POW"); sprintf(original[counter].parsed, "(0)%c", '^'); counter++; continue; }

else if(counter == 0) { original[counter].initial = strdup("SIN"); sprintf(original[counter].parsed, "(0)%c", 'S'); counter++; continue; }

else if(counter == 1) { original[counter].initial = strdup("LN"); sprintf(original[counter].parsed, "(0)%c", 'L'); counter++; continue; }

else if(counter == 2) { original[counter].initial = strdup("COS"); sprintf(original[counter].parsed, "(0)%c", 'C'); counter++; continue; }

else if(counter == 3) { original[counter].initial = strdup("TAN"); sprintf(original[counter].parsed, "(0)%c", 'T'); counter++; continue; }

else if(counter == 4) { original[counter].initial = strdup("SH"); sprintf(original[counter].parsed, "(0)%c", 's'); counter++; continue; }

else if(counter == 5) { original[counter].initial = strdup("CH"); sprintf(original[counter].parsed, "(0)%c", 'c'); counter++; continue; }

else if(counter == 6) { original[counter].initial = strdup("TH"); sprintf(original[counter].parsed, "(0)%c", 't'); counter++; continue; }

else if(counter == 7) { original[counter].initial = strdup("ARCTG"); sprintf(original[counter].parsed, "(0)%c", '&'); counter++; continue; }

else if(counter == 8) { original[counter].initial = strdup("ARCCOS"); sprintf(original[counter].parsed, "(0)%c", 'W'); counter++; continue; }
# 45 "./src/matan_killer_f.cpp" 2
        break;
    }
# 56 "./src/matan_killer_f.cpp"
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



char * replaceFuncNames(const char * input)
{

    Lex_sub * lex = getLexicalSubstitusions();

    char * result = nullptr;
    const char * searched = nullptr;

    for (int idx = 0; idx < MAX_FUNC_NUMBER; idx++)
    {

        if (lex[idx].initial == nullptr)
            break;


        searched = strstr(input, lex[idx].initial);

        if (searched != nullptr)
        {
            size_t initial_len = strlen(lex[idx].initial);
            size_t parsed_len = strlen(lex[idx].parsed);
            size_t input_len = strlen(input);

            size_t initial_shift = searched - input;
            size_t remainder_len = input_len - initial_shift - initial_len;






            result = (char *)calloc(initial_shift + parsed_len + remainder_len + 1, sizeof(char));
# 117 "./src/matan_killer_f.cpp"
            strncpy(result, input, initial_shift);




            strncat(result, lex[idx].parsed, parsed_len);




            strncat(result, searched + initial_len, remainder_len);




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





    const char * open_bracket = strchr(&input[shift], '(');
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





    size_t parsing_length = first_bracket - &input[shift];





    if (parsing_length > 0)
    {
        parseTerminalNode(exp_node, &input[shift], parsing_length);





        readExpression(exp_node, input, shift + parsing_length, free_port);

    } else if (&input[shift] == close_bracket)
    {
        readExpression(exp_node->parent, input, shift + 1, (free_port + 1) % 2);

    } else if (&input[shift] == open_bracket)
    {

        Exp_node * new_node = nodeConnect(exp_node, free_port);
        readExpression(new_node, input, shift + 1, free_port );

    }else{

        readExpression(exp_node, input, shift + 1, free_port);
    }

    return 0;
}







Operator isOp(int symbol)
{
    Operator result = NOT_OP;

    if (0)
    { }
# 1 "./lib/operations.h" 1
# 11 "./lib/operations.h"
else if(symbol == '\r') { result = NOT_OP; }

else if(symbol == '+') { result = ADD; }

else if(symbol == '-') { result = SUB; }

else if(symbol == '*') { result = MUL; }

else if(symbol == '/') { result = DIV; }

else if(symbol == '^') { result = POW; }

else if(symbol == 'S') { result = SIN; }

else if(symbol == 'L') { result = LN; }

else if(symbol == 'C') { result = COS; }

else if(symbol == 'T') { result = TAN; }

else if(symbol == 's') { result = SH; }

else if(symbol == 'c') { result = CH; }

else if(symbol == 't') { result = TH; }

else if(symbol == '&') { result = ARCTG; }

else if(symbol == 'W') { result = ARCCOS; }
# 263 "./src/matan_killer_f.cpp" 2

    return result;
}



bool isTerminal(Exp_node *node)
{
    if (!node->l_son && !node->r_son)
    {
        return true;

    }else {

        return false;
    }

}

bool hasNumSons(Exp_node *node)
{

    assert(isTerminal(node));

    if (node->l_son->type == NUM && node->r_son->type == NUM)
    {
        return true;

    } else {

        return false;
    }
}

Exp_node * simplifyTree(Exp_node *node)
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
    }

    if (node->l_son)
    {
        Exp_node *left_result = simplifyTree(node->l_son);

        if (result->l_son != left_result)
        {
            result->l_son = left_result;
            linkToParent(result, left_result);
        }
    }

    if (node->r_son)
    {
        Exp_node *right_result = simplifyTree(node->r_son);

        if (result->r_son != right_result)
        {
            result->r_son = right_result;
            linkToParent(result, right_result);
        }
    }

    return result;

}
# 349 "./src/matan_killer_f.cpp"
Exp_node * rollUpTree(Exp_node *node)
{
    bool have_to_roll_up = false;
    Value val = {};
    Exp_node * new_node = node;

    if (0) {}
# 1 "./lib/operations.h" 1
# 11 "./lib/operations.h"
else if (node->value.op_value == '\r' && -2 == MAGIC) { val.dbl_value = (node->l_son->value.dbl_value) '\r' (node->r_son->value.dbl_value); have_to_roll_up = true; }

else if (node->value.op_value == '+' && -1 == MAGIC) { val.dbl_value = (node->l_son->value.dbl_value) '+' (node->r_son->value.dbl_value); have_to_roll_up = true; }

else if (node->value.op_value == '-' && -1 == MAGIC) { val.dbl_value = (node->l_son->value.dbl_value) '-' (node->r_son->value.dbl_value); have_to_roll_up = true; }

else if (node->value.op_value == '*' && -1 == MAGIC) { val.dbl_value = (node->l_son->value.dbl_value) '*' (node->r_son->value.dbl_value); have_to_roll_up = true; }

else if (node->value.op_value == '/' && -1 == MAGIC) { val.dbl_value = (node->l_son->value.dbl_value) '/' (node->r_son->value.dbl_value); have_to_roll_up = true; }

else if (node->value.op_value == '^' && -3 == MAGIC) { val.dbl_value = (node->l_son->value.dbl_value) '^' (node->r_son->value.dbl_value); have_to_roll_up = true; }

else if (node->value.op_value == 'S' && 0 == MAGIC) { val.dbl_value = (node->l_son->value.dbl_value) 'S' (node->r_son->value.dbl_value); have_to_roll_up = true; }

else if (node->value.op_value == 'L' && 1 == MAGIC) { val.dbl_value = (node->l_son->value.dbl_value) 'L' (node->r_son->value.dbl_value); have_to_roll_up = true; }

else if (node->value.op_value == 'C' && 2 == MAGIC) { val.dbl_value = (node->l_son->value.dbl_value) 'C' (node->r_son->value.dbl_value); have_to_roll_up = true; }

else if (node->value.op_value == 'T' && 3 == MAGIC) { val.dbl_value = (node->l_son->value.dbl_value) 'T' (node->r_son->value.dbl_value); have_to_roll_up = true; }

else if (node->value.op_value == 's' && 4 == MAGIC) { val.dbl_value = (node->l_son->value.dbl_value) 's' (node->r_son->value.dbl_value); have_to_roll_up = true; }

else if (node->value.op_value == 'c' && 5 == MAGIC) { val.dbl_value = (node->l_son->value.dbl_value) 'c' (node->r_son->value.dbl_value); have_to_roll_up = true; }

else if (node->value.op_value == 't' && 6 == MAGIC) { val.dbl_value = (node->l_son->value.dbl_value) 't' (node->r_son->value.dbl_value); have_to_roll_up = true; }

else if (node->value.op_value == '&' && 7 == MAGIC) { val.dbl_value = (node->l_son->value.dbl_value) '&' (node->r_son->value.dbl_value); have_to_roll_up = true; }

else if (node->value.op_value == 'W' && 8 == MAGIC) { val.dbl_value = (node->l_son->value.dbl_value) 'W' (node->r_son->value.dbl_value); have_to_roll_up = true; }
# 357 "./src/matan_killer_f.cpp" 2

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



int parseTerminalNode(Exp_node *exp_node, const char * parsing_start, size_t parsing_length)
{

    char * terminalValue = strdup(parsing_start);
    terminalValue[parsing_length] = '\0';





    double value;
    char char_value = ' ';

    if (sscanf(terminalValue, "%lf", &value) == 1)
    {
        exp_node->type = NUM;
        exp_node->value.dbl_value = value;
        free(terminalValue);
        return 0;

    }else
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



                    leftOp(MUL);
                    rightOp(MUL);



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


                        d(LEFT_SON, LEFT_SON);
                        rightOp(MUL);



                        cR(LEFT_SON, RIGHT_SON);
                        right_right_Op(POW);



                        cRR(LEFT_SON, LEFT_SON);
                        right_right_right_Op(SUB);



                        cRRR(LEFT_SON, RIGHT_SON);
                        mRRR(RIGHT_SON, 1);

                    }else {
# 526 "./src/matan_killer_f.cpp"
                    }

                    break;
                }
                case DIV:
                {
                    makeOp(DIV);



                    leftOp(SUB);
                    rightOp(POW);



                    left_left_Op(MUL);
                    left_right_Op(MUL);

                    cR(LEFT_SON,RIGHT_SON);
                    mR(RIGHT_SON, 2);



                    dLL(LEFT_SON, LEFT_SON);
                    cLL(RIGHT_SON, RIGHT_SON);

                    dLR(LEFT_SON, RIGHT_SON);
                    cLR(RIGHT_SON, LEFT_SON);

                    break;
                }
                case SIN:
                {
                    makeOp(MUL);


                    d(RIGHT_SON, RIGHT_SON);
                    leftOp(COS);



                    mL(LEFT_SON, 0);
                    cL(RIGHT_SON, RIGHT_SON);

                    break;
                }
                case COS:
                {
                    makeOp(MUL);



                    leftOp(MUL);
                    d(RIGHT_SON, RIGHT_SON);



                    mL(LEFT_SON, -1);
                    left_right_Op(SIN);


                    cLR(RIGHT_SON, RIGHT_SON);
                    mLR(LEFT_SON, 0);

                    break;
                }
                case LN:
                {
                    makeOp(MUL);



                    leftOp(DIV);
                    d(RIGHT_SON, RIGHT_SON);



                    mL(LEFT_SON, 1);
                    cL(RIGHT_SON, RIGHT_SON);

                    break;
                }
                case TAN:
                {

                    break;
                }
                default:
                    printf("go f smbdy");
                    break;
            }
            break;
    }




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
