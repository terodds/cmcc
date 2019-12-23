#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include <string.h>

static void declarationError(TreeNode *t, char *message);

// 先序遍历语法分析树
static void traverse(TreeNode *t,
    void (* preProc)(TreeNode *),
    void (* postProc)(TreeNode *));
{
    if(t!=NULL)
    {
        preProc(t);
        {   int i;
            for(i=0;i<MAXCHILDLEN;i++)  traverse(t->child[i],preProc, postProc);
        }
        postProc(t);
        traverse(t->sibling, preProc, postProc);
    }
}

static void nullProc(TreeNode *t)
{
    if(t==NULL) return;
    else return;
}

static int number_of_parameters(TreeNode *t)
{
    int count = 0;
    TreeNode *aux = t->child[0];
    while(aux != NULL)
    {
        count++;
        aux = aux->sibling;
    }
    return count;
}

static void check_vector_operation(TreeNode *t, char *message)
{

}

static void insertNode(TreeNode *t)
{

}

static void mainError()
{

}

void buildSymtab(TreeNode *syntaxTree, int prog)
{
    
}

static void declarationError(TreeNode *t, char *message)
{

}

static void checkNode(TreeNode *t)
{

}

void typeCheck(TreeNode *syntaxTree)
{
    traverse(syntaxTree, nullProc, checkNode);
}
