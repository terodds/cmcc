#ifndef _TREE_H_
#define _TREE_H_


/*********************
 * 语法分析树-数据结构
 */


typedef enum {
    DECL_L, DECL, VAR_DECL, 
    TYPE, FUN_DECL, PARAM,
    PARAM_VAR, PARAM_ARR, 
    CMPD_STMT, EMPTY, EXPR_STMT,
    SELECT_STMT, ITER_STMT, RET_STMT, 
    ASSIGN_EXPR, ARRAY_EL, REL_EXPR, 
    ADD_EXPR, MUL_EXPR, CALL
} NodeKind;

struct array_attr
{
    char *name;
    int size;
};

typedef int attr_t;
typedef struct TreeNode
{
    int nodekind;
    attr_t attr;
    struct TreeNode *firstchild, *nextsibling;   
} TreeNode, *ParserTree;


TreeNode *addSibling(TreeNode *, TreeNode *);
TreeNode *mknode(int, TreeNode *);

//void printTree(FILE *, TreeNode *);
void visitNode(TreeNode *);
void printTree(TreeNode *);



#endif
