#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#include "matan_killer_f.h"
#include "matan_killer_debug.h"

int processSelectedMode(Exp_node * initial_tree, Var v_arr [])
{
    char menu[MAX_BUFFER_LENGTH] = "Выберите режим запуска программы\n"
                                    "\t1 - Взять n-ю производную\n"
                                    "\t2 - Взять частную производную\n"
                                    "\t3 - Посчитать значение функции в точке\n"
                                    "\t4 - Разложить функцию в ряд Маклорена до n-го порядка\n"
                                    "\t5 - Упростить выражение\n"
                                    "\t6 - Выйти\n";

    int chosen_mode = 0;       

    do {

        writeInColor(menu, 'g');
        
        if (scanf(" %d", &chosen_mode) != 1)
        {
                scanf( "%*[^\n]" );
                continue;
        }

        switch(chosen_mode)
        {
            case DIFF:
            {           
                size_t diff_times = 0;

                Exp_node * diff_func = differentiate_n_times(initial_tree, &diff_times);
                simplifyTree(&diff_func);

                char info[MAX_BUFFER_LENGTH] = {};
                sprintf(info, "after %lu differentiation", diff_times);

                TREE_DUMP_OPTIONAL(diff_func, info);
                printIn(diff_func);
                printf("\n");

                nodeDtor(&diff_func);

                break;
            }
            case DIFF_PARTIALY:
            {                    
                //TODO add partial differentiate more then once
                Exp_node * diff_func = differentiatePartialy(initial_tree, v_arr);
                simplifyTree(&diff_func);

                TREE_DUMP_OPTIONAL(diff_func, "after partial differentiation");
                printIn(diff_func);
                printf("\n");
                nodeDtor(&diff_func);

                break;
            }
            case COMPUTE_IN_POINT:
            {
                //TODO add info about place wher the function was differentiated
                printf("Calculated = %g \n", calculateTree(copy(initial_tree), v_arr));
                
                break;
            }
            case ARRANGE_IN_A_ROW:
            {
                
                break;
            }
            case SIMPLIFY:
            {
                simplifyTree(&initial_tree);

                TREE_DUMP_OPTIONAL(initial_tree, "after simplification");
                printIn(initial_tree);
                printf("\n");

                break;
            }
            case QUIT:
            {
                break;
            }
            default:
            {
                writeInColor("TYPE AGAIN USER\n", 'r');
                break;                
            }

        }

        // printf("%d \n", chosen_mode);

    } while (chosen_mode != QUIT);

    printf("You quit!\n");

    return 0;

}

Exp_node *arrangeInTeilorRow(const Exp_node *node, Var v_arr[], size_t range)
{
    // if (range == 0)
    // {

    // }

    // Exp_node * copy_node = copy(node);

    // double * ratios = (double *)calloc(range + 1, sizeof(double));

    // fillVarValues(v_arr);
    // substitudeVariables(copy_node, v_arr, NULL);

    // for (int idx = 1; idx <= range; idx++)
    // {


    // }

    // free(ratios);
    return nullptr;

}

Exp_node * differentiate_n_times(const Exp_node *node, size_t * number)
{
    while (1)
    {
        printf("How many times you want to take the derivative?\n");

        if (scanf(" %lu", number) != 1)
        {
                scanf( "%*[^\n]" );
                continue;
        }

        Exp_node * last_diff = copy(node);

        saveMicroTransform(node);
        saveMicroTransform(last_diff);

        if (*number == 0)
        {   
            return last_diff;

        } else 
        {
            for (size_t counter = 1; counter <= *number; counter++)
            {
                Exp_node * new_diff = differentiate(last_diff);
                
                saveMicroTransform(new_diff);

                nodeDtor(last_diff);

                if (!hasVariable(new_diff))
                {
                    nodeDtor(&new_diff);
                    DBG_OUT;
                    break;
                }

                copyNodeData(new_diff, last_diff); 

                nodeDtor(&new_diff);


                char line[MAX_BUFFER_LENGTH] = {};
                sprintf(line, "after %ld differentiation", counter);

                TREE_DUMP_OPTIONAL(last_diff, line);

            }
        
            return last_diff;
        }

        break;
    }

    return nullptr;
}

Exp_node * differentiatePartialy(const Exp_node *node, Var v_arr[])
{
    if (!hasVariable(node))
    {
        printf("Can't differentiate num expression!\n");
        return NULL;
    }

    fillVarValues(v_arr);
    // dumpVarArray(v_arr);
    // DBG_OUT;
    
    char var_name[MAX_BUFFER_LENGTH] = {};

    do {

        printf("Enter name of variable that you want to partialy differentiate: \n");
        fscanf(stdin, "%*[\n]" );
        scanf("%[^\n]s", var_name);
        // STRING_DUMP(var_name);
        
    } while (!checkExistence(v_arr, var_name));

    Exp_node * node_copy = copy(node);
    
    substitudeVariables(node_copy, v_arr, var_name);
    TREE_DUMP_OPTIONAL(node_copy, "after variable replacement");

    Exp_node * diff_result = differentiate(node_copy);
    nodeDtor(&node_copy);
    
    //TODO clear v_arr
    return diff_result;
}

double calculateTree(Exp_node *node, Var v_arr[])
{
    fillVarValues(v_arr);

    // printf("%g\n", v_arr[0].value);

    substitudeVariables(node, v_arr, NULL);
    // TREE_DUMP_OPTIONAL(node, "Our tree after substitution");

    if (hasVariable(node))
    {
        printf("YOU HAVE A VARIABLE IN CALCUlATED TREE!\n");
        return 0;
    }

    calculateNumTree(node);

    double result = node->value.dbl_value;

    free(node);

    return result;

}

int simplifyTree(Exp_node **node)
{
    // ASSERT(node);

    int changes = 0;

    int tmp_changes = 0;

    do {    

        changes = 0;

        changes += wrapEquivalents(*node);

        if (tmp_changes != changes)
        {
            saveMicroTransform(*node);
            tmp_changes = changes;
        }

        changes += computeConstants(*node);

        if (tmp_changes != changes)
        {
            saveMicroTransform(*node);
            tmp_changes = 0;
        }

    } while (changes != 0);

    return 0;
}

int writeInColor(const char * sentence, char color)
{   
    char command[MAX_BUFFER_LENGTH] = {};

    switch(color)
    {
        case 'w':
            fprintf(stdout, "\e[0m%s\e[0m", sentence);        
            break;
        case 'g':
            fprintf(stdout, "\e[0;32m%s\e[0m", sentence);        
            break;        
        case 'r':
            fprintf(stdout, "\e[0;31m%s\e[0m", sentence);        
            break;  
        default :
            fprintf(stdout, "%s", sentence);        
            break;
    }

    return 0;
}