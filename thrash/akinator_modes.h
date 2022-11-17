#ifndef AK_MODES_H
#define AK_MODES_H

#include "akinator_f.h"

enum modes
{
    GUESS_OBJECT        = 1,
    DEFINE_OBJECT       = 2,
    COMPARE_TWO_OBJECTS = 3,
    DUMP_BASE           = 4,
    QUIT                = 5,
};

int addNode(Node * node);

int guessObject(Tree_t *tree);
int defineObject(Tree_t *tree);
int compareObjects(Tree_t *tree);

#endif 