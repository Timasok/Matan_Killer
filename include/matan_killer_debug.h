#ifndef MATAN_KILLER_DEBUG_H
#define MATAN_KILLER_DEBUG_H

#define DEF_ERR(name, num) \
            MATAN_KILLER_ERROR_##name = 1 << num,

enum MATAN_KILLER_ERRORS
{
        #include "errors.h"
};

#undef DEF_ERR
#endif