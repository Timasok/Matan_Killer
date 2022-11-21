//differentiated node will be linked with the main operation in new tree
#define d(linking_side, src_side)                                   \
        do{                                                         \
            diffNode(node, new_node, linking_side, src_side);       \
                                                                    \
        } while(0)                                                  \

//differentiated node will be linked with the left operation in new tree
#define dL(linking_side, src_side)                                  \
        do{                                                         \
            diffNode(node, new_node->l_son, linking_side, src_side);\
                                                                    \
        } while(0)                                                  \

//copied node will be linked with the left operation in new tree
#define cL(linking_side, src_side)                                  \
        do{                                                         \
            copyNode(node, new_node->l_son, linking_side, src_side);\
                                                                    \
        } while(0)                                                  \

//differentiated node will be linked with the right operation in new tree
#define dR(linking_side, src_side)                                  \
        do{                                                         \
            diffNode(node, new_node->r_son, linking_side, src_side);\
                                                                    \
        } while(0)                                                  \

//copied node will be linked with the right operation in new tree
#define cR(linking_side, src_side)                                  \
        do{                                                         \
            copyNode(node, new_node->r_son, linking_side, src_side);\
                                                                    \
        } while(0)                                                  \

#define cRR(linking_side, src_side)                                                                 \
        do{                                                                                         \
            copyNode(node, new_node->r_son->r_son, linking_side, src_side);                         \
                                                                                                    \
        } while(0)                                                                                  \

#define dLL(linking_side, src_side)                                                                 \
        do{                                                                                         \
            diffNode(node, new_node->l_son->l_son, linking_side, src_side);                         \
                                                                                                    \
        } while(0)                                                                                  \

#define dRL(linking_side, src_side)                                                                 \
        do{                                                                                         \
            diffNode(node, new_node->r_son->l_son, linking_side, src_side);                         \
                                                                                                    \
        } while(0)                                                                                  \

#define cRL(linking_side, src_side)                                                                 \
        do{                                                                                         \
            copyNode(node, new_node->r_son->l_son, linking_side, src_side);                         \
                                                                                                    \
        } while(0)                                                                                  \

#define cLL(linking_side, src_side)                                                                 \
        do{                                                                                         \
            copyNode(node, new_node->l_son->l_son, linking_side, src_side);                         \
                                                                                                    \
        } while(0)                                                                                  \

#define dLR(linking_side, src_side)                                                                 \
        do{                                                                                         \
            diffNode(node, new_node->l_son->r_son, linking_side, src_side);                         \
                                                                                                    \
        } while(0)                                                                                  \

#define cLR(linking_side, src_side)                                                                 \
        do{                                                                                         \
            copyNode(node, new_node->l_son->r_son, linking_side, src_side);                         \
                                                                                                    \
        } while(0)                                                                                  \


#define cRRR(linking_side, src_side)                                                                \
        do{                                                                                         \
            copyNode(node, new_node->r_son->r_son->r_son, linking_side, src_side);                  \
                                                                                                    \
        } while(0)                                                                                  \

#define m(linking_side, number)                                                                     \
        do{                                                                                         \
                switch (linking_side)                                                               \
                {                                                                                   \
                        case LEFT_SON:                                                              \
                                new_node->l_son = createNum(number);                                \
                                break;                                                              \
                        case RIGHT_SON:                                                             \
                                new_node->r_son = createNum(number);                                \
                                break;                                                              \
                        default:                                                                    \
                                DBG_OUT;                                                            \
                                break;                                                              \
                }                                                                                   \
                                                                                                    \
                linkSonsToParent(new_node);                                                         \
        } while(0)                                                                                  \


#define mL(linking_side, number)                                                                    \
        do{                                                                                         \
                switch (linking_side)                                                               \
                {                                                                                   \
                        case LEFT_SON:                                                              \
                                new_node->l_son->l_son = createNum(number);                         \
                                break;                                                              \
                        case RIGHT_SON:                                                             \
                                new_node->l_son->r_son = createNum(number);                         \
                                break;                                                              \
                        default:                                                                    \
                                DBG_OUT;                                                            \
                                break;                                                              \
                }                                                                                   \
                                                                                                    \
                linkSonsToParent(new_node->l_son);                                                  \
        } while(0)                                                                                  \


#define mR(linking_side, number)                                                                    \
        do{                                                                                         \
                switch (linking_side)                                                               \
                {                                                                                   \
                        case LEFT_SON:                                                              \
                                new_node->r_son->l_son = createNum(number);                         \
                                break;                                                              \
                        case RIGHT_SON:                                                             \
                                new_node->r_son->r_son = createNum(number);                         \
                                break;                                                              \
                        default:                                                                    \
                                DBG_OUT;                                                            \
                                break;                                                              \
                }                                                                                   \
                                                                                                    \
                linkSonsToParent(new_node->r_son);                                                  \
        } while(0)                                                                                  \

#define mRL(linking_side, number)                                                                   \
        do{                                                                                         \
                switch (linking_side)                                                               \
                {                                                                                   \
                        case LEFT_SON:                                                              \
                                new_node->r_son->l_son->l_son = createNum(number);                  \
                                break;                                                              \
                        case RIGHT_SON:                                                             \
                                new_node->r_son->l_son->r_son = createNum(number);                  \
                                break;                                                              \
                        default:                                                                    \
                                DBG_OUT;                                                            \
                                break;                                                              \
                }                                                                                   \
                                                                                                    \
                linkSonsToParent(new_node->r_son->l_son);                                           \
        } while(0)                                                                                  \

#define mLR(linking_side, number)                                                                   \
        do{                                                                                         \
                switch (linking_side)                                                               \
                {                                                                                   \
                        case LEFT_SON:                                                              \
                                new_node->l_son->r_son->l_son = createNum(number);                  \
                                break;                                                              \
                        case RIGHT_SON:                                                             \
                                new_node->l_son->r_son->r_son = createNum(number);                  \
                                break;                                                              \
                        default:                                                                    \
                                DBG_OUT;                                                            \
                                break;                                                              \
                }                                                                                   \
                                                                                                    \
                linkSonsToParent(new_node->l_son->r_son);                                           \
        } while(0)                                                                                  \


#define mRRR(linking_side, number)                                                                  \
        do{                                                                                         \
                switch (linking_side)                                                               \
                {                                                                                   \
                        case LEFT_SON:                                                              \
                                new_node->r_son->r_son->r_son->l_son = createNum(number);           \
                                break;                                                              \
                        case RIGHT_SON:                                                             \
                                new_node->r_son->r_son->r_son->r_son = createNum(number);           \
                                break;                                                              \
                        default:                                                                    \
                                DBG_OUT;                                                            \
                                break;                                                              \
                }                                                                                   \
                                                                                                    \
                linkSonsToParent(new_node->r_son->r_son->r_son);                                    \
        } while(0)                                                                                  \


#define copyOp()                                                \
        do{                                                     \
            copyNodeData(node, new_node);                       \
        } while(0)     

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

#define left_left_Op(operation)                                        \
        do{                                                            \
            new_node->l_son->l_son = createOp(operation);              \
            linkToParent(new_node->l_son, new_node->l_son->l_son);     \
        } while(0)

#define left_right_Op(operation)                                       \
        do{                                                            \
            new_node->l_son->r_son = createOp(operation);              \
            linkToParent(new_node->l_son, new_node->l_son->r_son);     \
        } while(0)

#define right_left_Op(operation)                                       \
        do{                                                            \
            new_node->r_son->l_son = createOp(operation);              \
            linkToParent(new_node->r_son, new_node->r_son->l_son);     \
        } while(0)

//TODO add assert that checks if we don't work with deep nodes unless we difine the upper ones
#define right_right_Op(operation)                                   \
        do{                                                         \
            new_node->r_son->r_son = createOp(operation);           \
            linkToParent(new_node->r_son, new_node->r_son->r_son);  \
        } while(0)

#define right_right_right_Op(operation)                             \
        do{                                                         \
            new_node->r_son->r_son->r_son = createOp(operation);    \
            linkToParent(new_node->r_son->r_son, new_node->r_son->r_son->r_son);  \
        } while(0)    

#define rightOp(operation)                                      \
        do{                                                     \
            new_node->r_son = createOp(operation);              \
            linkToParent(new_node, new_node->r_son);            \
        } while(0)                                              