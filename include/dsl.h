//differentiated node will be linked with the main operation in new tree
#define d(linking_side, src_side)                                   \
        do{                                                         \
            diffNode(node, new_node, linking_side, src_side);       \
                                                                    \
        } while(0)    

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

#define rightOp(operation)                                      \
        do{                                                     \
            new_node->r_son = createOp(operation);              \
            linkToParent(new_node, new_node->r_son);            \
        } while(0)                                              