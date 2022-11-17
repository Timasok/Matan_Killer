#include <string.h>
#include <unistd.h>

#include "akinator_modes.h"
#include "akinator_debug.h"

// sstatic int nodeNumber = 0;
static bool guessed = false;
static const Node * guessedNode = nullptr;
const int MAX_DEPTH = 20;

//TODO add to text func замещение конца

static int printAnswerInfo()
{
    printf("[y\\n]\n");
    return 0;
}

static int getAnswer(const Node * node, char *answer)
{

    printAnswerInfo();
   
    int symbol = getchar(); 
    
    if (symbol != '\n')
        ungetc(symbol, stdin);
      
    *answer = getchar();

    while (getchar() != '\n')
        ;

    if (*answer == 'y' || *answer == 'n')
    {
        return 0;
    }else{
        sayAndWrite("Ошибка ввода повторите\n", 'r');
        getAnswer(node, answer);
    }

    return 0;    
}

static int printQuestion(const char * string)
{
    char line_to_say[MAX_BUFFER_LENGTH] = {};
    sprintf(line_to_say, "%s?", string);
    sayAndWrite(line_to_say, 'g');

    return 0;
}

static int processGuessed(Node *node)
{
    guessedNode = node;
    sayAndWrite("Это ", 'g');
    printQuestion(node->data);
    
    char answer = ' ';
    getAnswer(node, &answer);
    if(answer == 'n')
    {
        addNode(node);
        sayAndWrite("Добавил в базу! Больше с этим не проведёте\n", 'w');         
    }else
    {
        sayAndWrite("Эхху! Я угадал блэт! Давай ещё!!!\n", 'w');
    }

    return 0;
}

int addNode(Node * node)
{
    if (node->l_son != nullptr || node->r_son != nullptr)
        return AK_ERROR_ADDING_NODE_TO_PARENT;

    char correct_object[MAX_BUFFER_LENGTH]      = {};
    char clarifying_question[MAX_BUFFER_LENGTH] = {};
    int len = 0;

    sayAndWrite("Введите правильный объект\n", 'g');

    fgets(correct_object, MAX_BUFFER_LENGTH, stdin);
    strchr(correct_object, '\n')[0] = '\0';
    // printf("reading %d\n", correct_object[0]);

    char line_to_say[MAX_BUFFER_LENGTH] = {};
    sprintf(line_to_say, "Чем %s отличается от %s\n", correct_object, node->data);
    sayAndWrite(line_to_say, 'g');

    fgets(clarifying_question, MAX_BUFFER_LENGTH, stdin);
    strchr(clarifying_question, '\n')[0] = '\0';

    // fprintf(stdout, "correct object %s clarifying question %s\n",correct_object, clarifying_question);

    Node * incorrect_value = nodeConnect(node, RIGHT_SON);
    Node * correct_value = nodeConnect(node, LEFT_SON);

    incorrect_value->data = strdup(node->data);
    correct_value->data = strdup(correct_object);
    node->data = strdup(clarifying_question);

    return 0;

}

static int getAnswerNext(Node* node)
{
    if (!node)
        return 0;

    char answer = ' ';
    // nodeDump(node);   

    if (node->l_son == nullptr && node->r_son == nullptr)
    {
        processGuessed(node);
        return 0;
    }    
    
    printQuestion(node->data);
    getAnswer(node, &answer);

    switch(answer)
    {
        case 'y':
            getAnswerNext(node->l_son);
            break;
        case 'n':
            getAnswerNext(node->r_son);
            break;
        default:
            break;
    }

    return 0;

}

static int getPreviousPhrase(Node* node, Node* primal_node)
{
    if (!node || node == primal_node)
        return 0;

    // printf("\e[0;31m%s\e[0m", node->data);

    char line_to_say[MAX_BUFFER_LENGTH] = {};

    if (node->l_son == nullptr && node->r_son == nullptr)
    {
        sprintf(line_to_say, "%s - это", node->data);
       
    }else
    {
        sprintf(line_to_say, " %s", node->data);

    }
        sayAndWrite(line_to_say, 'w'); 

    if (node->parent->l_son == node)
    {
        getPreviousPhrase(node->parent, primal_node);

    }

    if (node->parent->r_son == node)
    {
        sayAndWrite(" не", 'w'); 
        getPreviousPhrase(node->parent, primal_node);

    }

    return 0;

}

//TODO strange 1 function to do one function
int guessObject(Tree_t *tree){

    getAnswerNext(tree->main_node->l_son);
    return 0;
}

int defineObject(Tree_t *tree)
{
    char object_data[MAX_BUFFER_LENGTH] = {};

    sayAndWrite("Введите объект который хотите найти в базе\n", 'g');
    fscanf(stdin, "%*[\n]" );
    fprintf(stdin,"\n");

    fscanf(stdin,"%[^\n]s", object_data);

    Node *correct_node = findNode(tree->main_node->l_son, object_data);

    if (correct_node == nullptr)
    {
        char line_to_say[MAX_BUFFER_LENGTH] = {};
        sprintf(line_to_say, "Объект %s не найден в базе. Сделайте другой запрос.\n", object_data);
        sayAndWrite(line_to_say, 'r');
    
    }else
    {
        getPreviousPhrase(correct_node, tree->main_node);
        sayAndWrite("\n", 'w');
    }

    return 0;
}

int compareObjects(Tree_t *tree)
{
    char first_object[MAX_BUFFER_LENGTH]  = {};
    char second_object[MAX_BUFFER_LENGTH] = {};
    char line_to_say[MAX_BUFFER_LENGTH] = {};

    sayAndWrite("Введите первый объект\n", 'g');
    fscanf(stdin, "%*[\n]" );
    fprintf(stdin,"\n");
    scanf("%[^\n]s", first_object);      
    
    sayAndWrite("Введите второй объект\n", 'g');
    fscanf(stdin, "%*[\n]" );
    fprintf(stdin,"\n");

    scanf("%[^\n]s", second_object);

    Node *first_node = findNode(tree->main_node->l_son, first_object);
    Node *second_node = findNode(tree->main_node->l_son, second_object);
    
    if (nodeAssert(first_node, first_object) || nodeAssert(second_node, second_object))
    {
        return -1;

    }else{
        Node *answers_one[MAX_DEPTH] = {};
        Node *answers_two[MAX_DEPTH] = {};

        size_t first_counter  = 0;
        size_t second_counter = 0;

        answers_one[first_counter]  = first_node;
        answers_two[second_counter] = second_node;
        bool stop_searching = false;

        while (stop_searching == false)
        {
            // printf("added recently: %p %p\n", answers_one[first_counter], answers_two[second_counter]);
            if (answers_one[first_counter] != nullptr)
            {
                first_counter++;
                answers_one[first_counter]  = answers_one[first_counter-1]->parent;
            }

            if (answers_two[second_counter] != nullptr)
            {
                second_counter++;
                answers_two[second_counter] = answers_two[second_counter-1]->parent;                
            
            }
            
            if (answers_one[first_counter] == nullptr && answers_two[second_counter] == nullptr)
            {
                stop_searching = true;
            }
            
        }

        sprintf(line_to_say, "%s похож на %s тем, что \n\t", first_object, second_object);
        sayAndWrite(line_to_say, 'w');

        first_counter--;
        second_counter--;
        while (first_counter > 0 && second_counter > 0)
        {

            first_counter--;
            second_counter--;    
            
            if (answers_one[first_counter] == answers_two[second_counter])
            {   
                if (answers_one[first_counter-1] == answers_one[first_counter]->r_son )
                    sayAndWrite("не ", 'w');
                
                sprintf(line_to_say, "%s ", answers_one[first_counter]->data);
                sayAndWrite(line_to_say, 'w');
                
            }else
            {
                break;
            }
            
        }

        sleep(1);
        sprintf(line_to_say, "\nНо %s \n\t", first_object);
        sayAndWrite(line_to_say, 'w');

        for (;first_counter > 0; first_counter--)
        {
            if (answers_one[first_counter-1] == answers_one[first_counter]->r_son)
                sayAndWrite("не ", 'w');
            
            sprintf(line_to_say, "%s ", answers_one[first_counter]->data);
            sayAndWrite(line_to_say, 'w');
            
        }
        
        sleep(1);
        sprintf(line_to_say, "\nА %s \n\t", second_object);
        sayAndWrite(line_to_say, 'w');

        for (;second_counter > 0; second_counter--)
        {
            if (answers_two[second_counter-1] == answers_two[second_counter]->r_son)
                sayAndWrite("не ", 'w');
            
            sprintf(line_to_say, "%s ", answers_two[second_counter]->data);
            sayAndWrite(line_to_say, 'w');
            
        }

        sayAndWrite("\n", 'w');

        return 0;  

    }    

}