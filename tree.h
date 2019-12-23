#ifndef _TREE_H_
#define _TREE_H_

#include "globals.h"

/*********************
 * 语法分析树-数据结构
 */

typedef enum {Empty, IntConst, String, Temp, Label} OperandKind;

typedef struct
{
    OperandKind kind;
    union {
        int val;
        struct
        {
            const char *name;
        } variable;
    } contents;
} Operand;

typedef enum
{
    StmtK,
    ExpK
} NodeKind;
typedef enum
{
    IfK,
    WhileK,
    AssignK,
    ReturnK,
    FuncK,
    CallK,
    ParamsK
} StmtKind;

typedef enum
{
    OpK,
    ConstK,
    IdK,
    TypeK,
    VectorK
} ExpKind;

typedef enum {Void,Integer,Boolean} ExpType;

typedef struct TreeNode
{
    struct TreeNode *firstchild, *nextsibling;
    int lineno;
    int already_seem;
    NodeKind nodekind;
    union {
        StmtKind stmt;
        ExpKind exp;
    } kind;
    union {
        TokenType op;
        int val;
        char *name;
    } attr;
    char *scope;
    Operand *oper;
    int is_vector;
    ExpType type;
} TreeNode;

TreeNode *addSibling(TreeNode *, TreeNode *);
TreeNode *mknode(int, TreeNode *);
TreeNode *addChild(TreeNode *parent, TreeNode *child);

TreeNode *newStmtNode(StmtKind);

TreeNode *newExpNode(ExpKind);

char *copyString(char *);

void printTree(TreeNode *);

#endif
