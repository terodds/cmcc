#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

TreeNode *mknode(int nodekind, TreeNode *firstchild)
{
    TreeNode *t = (TreeNode *)calloc(1, sizeof(TreeNode));
    t->nodekind = nodekind;
    t->firstchild = firstchild;
    t->nextsibling = NULL;
    return t;
}

TreeNode *addSibling(TreeNode *origin, TreeNode *follow)
{
     if (origin != NULL) {
    TreeNode *t = origin;
    while (t->nextsibling != NULL) t = t->nextsibling;
    t->nextsibling = follow;
  }
  else {
    origin = follow;
  }
  return origin;    
}

void visitNode(TreeNode *n)
{
    static int depth = 0;
    TreeNode *p = n;
    char padding[50] = {0};
    memset(padding, 0, 50);
    memset(padding, ' ', depth);
    if (p!=NULL)
    {
        printf("%snodekind: %d\n", padding, p->nodekind);   
        depth ++;
        visitNode(p->firstchild);
        depth --;
        visitNode(p->nextsibling);
    }
    else 
    {
        //printf("%s Null node\n", padding);
        return;
    }
    
}

void printTree(TreeNode *root)
{
    visitNode(root);
}
