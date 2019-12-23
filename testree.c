#include "globals.h"
#include "tree.h"

int main()
{
    TreeNode *root, *p1, *p2, *p3;
    p1 = mknode(1, NULL);
    p2 = mknode(2, NULL);
    p3 = mknode(3, p2);
    root = mknode(4, p1);
    addSibling(p1, p3);
    visitNode(root);
    return 0;
    
}
