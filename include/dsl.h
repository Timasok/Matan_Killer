
#define dL(side)                                                    \
        do{                                                         \
            switch(side)                                            \
            {                                                       \
                case LEFT_SON:                                      \
                    diffNode(node, new_node->l_son, LEFT_SON, LEFT_SON);      \
                    break;                                          \
                case RIGHT_SON:                                     \
                    diffNode(node, new_node->l_son, RIGHT_SON, RIGHT_SON);     \
                    break;                                          \
                default:                                            \
                    printf("WRONG SIDE ");                          \
                    DBG_OUT;                                        \
                    break;                                          \
            }                                                       \
        } while(0)                                                  \

#define cL(side)                                                    \
        do{                                                         \
            switch(side)                                            \
            {                                                       \
                case LEFT_SON:                                      \
                    copyNode(node, new_node->l_son, LEFT_SON, LEFT_SON);      \
                    break;                                          \
                case RIGHT_SON:                                     \
                    copyNode(node, new_node->l_son, RIGHT_SON, RIGHT_SON);     \
                    break;                                          \
                default:                                            \
                    printf("WRONG SIDE ");                          \
                    DBG_OUT;                                        \
                    break;                                          \
            }                                                       \
        } while(0)                                                  \

#define dR(side)                                                    \
        do{                                                         \
            switch(side)                                            \
            {                                                       \
                case LEFT_SON:                                      \
                    diffNode(node, new_node->r_son, LEFT_SON, LEFT_SON);      \
                    break;                                          \
                case RIGHT_SON:                                     \
                    diffNode(node, new_node->r_son, RIGHT_SON, RIGHT_SON);     \
                    break;                                          \
                default:                                            \
                    printf("WRONG SIDE ");                          \
                    DBG_OUT;                                        \
                    break;                                          \
            }                                                       \
        } while(0)

#define cR(side)                                                    \
        do{                                                         \
            switch(side)                                            \
            {                                                       \
                case LEFT_SON:                                      \
                    copyNode(node, new_node->r_son, LEFT_SON, LEFT_SON);      \
                    break;                                          \
                case RIGHT_SON:                                     \
                    copyNode(node, new_node->r_son, RIGHT_SON, RIGHT_SON);     \
                    break;                                          \
                default:                                            \
                    printf("WRONG SIDE ");                          \
                    DBG_OUT;                                        \
                    break;                                          \
            }                                                       \
        } while(0)                                                  \

#define makeOp(operation)                                       \
        do{                                                     \
            new_node->type = OP;                                \
            new_node->value.op_value = operation;               \
        } while(0)      

#define leftOp(operation)                                       \
        do{                                                     \
            new_node->l_son = createOp(operation);              \
            linkToParent(new_node, new_node->l_son);            \
        } while(0)                                               

#define rightOp(operation)                                      \
        do{                                                     \
            new_node->r_son = createOp(operation);              \
            linkToParent(new_node, new_node->r_son);            \
        } while(0)                                              