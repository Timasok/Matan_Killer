#include <stdio.h>
#include <stdlib.h>

#include "text_funcs.h"
#include "akinator_modes.h"
#include "akinator_debug.h"

int main(int argc, char ** argv)
{
    Tree_t tree = {};

    Text_info txt = {};
    textCtor(&txt, "database.txt");
    printText(&txt);
    
    nodeCtor(&tree.main_node);

    readDataBase(&txt, 0, LEFT_SON, tree.main_node);
    textDtor(&txt);  

    char line_to_say[MAX_BUFFER_LENGTH] = {};
    sprintf(line_to_say, "Выберите режим запуска программы\n"
            "\t1 - Угадать объект\n"
            "\t2 - Определить объект\n"
            "\t3 - Сравнить объекты\n"
            "\t4 - Вывести базу\n"
            "\t5 - Выйти\n");

    TREE_DUMP(&tree);

    int chosen_mode = 0;        
    while (chosen_mode != QUIT)
    {
        char system_command[MAX_BUFFER_LENGTH] = {};
        bool base_changed = false;
        sayAndWrite(line_to_say, 'g');

        if (scanf(" %d", &chosen_mode) != 1)
        {
                scanf( "%*[^\n]" );
                continue;
        }

        switch(chosen_mode)
        {
            case GUESS_OBJECT:
                    guessObject(&tree);
                    base_changed = true;
                    break;
            case DEFINE_OBJECT:
                    defineObject(&tree);
                    break;
            case COMPARE_TWO_OBJECTS:
                    compareObjects(&tree);
                    break;
            case DUMP_BASE:
                    TREE_DUMP(&tree);
                    sprintf(system_command, "xdg-open graph_dumps/dump_%d.jpeg", getDumpNumber());
                    system(system_command);
                    break;
            case QUIT:
                    break;
            default:
                sayAndWrite("TYPE AGAIN USER\n", 'r');
                break;
        }

        if (base_changed == true)
        {
            saveBase(tree.main_node->l_son);
        }
    }

    treeDtor(&tree);
//     nodeDtor(tree.main_node);
    return 0;
}