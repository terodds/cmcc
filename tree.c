#include "tree.h"
#include "globals.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void printToken( TokenType token, const char* tokenString )
{ switch (token)
  { case IF:
    case ELSE:
    case INT:
    case WHILE:
    case RET:
    case VOID:
      fprintf(listing,
         "reserved word: %s\n",tokenString);
      break;
    case LT: fprintf(listing,"<\n"); break;
    case LE: fprintf(listing,"<=\n"); break;
    case GT: fprintf(listing,">\n"); break;
    case GE: fprintf(listing,">=\n"); break;
    case '=': fprintf(listing,"=\n"); break;
    case NEQ: fprintf(listing,"!=\n"); break;
    case EQ: fprintf(listing,"==\n"); break;
    case '(': fprintf(listing,"(\n"); break;
    case ')': fprintf(listing,")\n"); break;
    case '[': fprintf(listing,"[\n"); break;
    case ']': fprintf(listing,"]\n"); break;
    case '{': fprintf(listing,"{\n"); break;
    case '}': fprintf(listing,"}\n"); break;
    case ';': fprintf(listing,";\n"); break;
    case ',': fprintf(listing,",\n"); break;
    case '+': fprintf(listing,"+\n"); break;
    case '-': fprintf(listing,"-\n"); break;
    case '*': fprintf(listing,"*\n"); break;
    case '/': fprintf(listing,"/\n"); break;
    case ENDFILE: fprintf(listing,"EOF\n"); break;
    case NUM:
      fprintf(listing,
          "NUM, val= %s\n",tokenString);
      break;
    case ID:
      fprintf(listing,
          "ID, name= %s\n",tokenString);
      break;
    case ERROR:
      fprintf(listing,
          "ERROR: %s\n",tokenString);
      break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
}


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
    if (origin != NULL)
    {
        TreeNode *t = origin;
        while (t->nextsibling != NULL)
            t = t->nextsibling;
        t->nextsibling = follow;
    }
    else
    {
        origin = follow;
    }
    return origin;
}

TreeNode *addChild(TreeNode *parent, TreeNode *child)
{
    if (parent->firstchild == NULL)
        parent->firstchild = child;
    else
    {
        TreeNode *p = parent->firstchild;
        while (p->nextsibling != NULL)
            p = p->nextsibling;
        p->nextsibling = child;
    }
    return parent;
}

static char padding[100] = {0};

void printTree(TreeNode *tree)
{
    static int depth = 0;
    memset(padding, 0, 100);
    memset(padding, ' ', depth);
    if (tree != NULL)
    {
        printf("%s", padding);
        if (tree->nodekind == StmtK)
        {
            switch (tree->kind.stmt)
            {
            case IfK:
                fprintf(listing, "If\n");
                break;
            case WhileK:
                fprintf(listing, "While\n");
                break;
            case AssignK:
                fprintf(listing, "Assign:\n");
                break;
            case ReturnK:
                fprintf(listing, "Return\n");
                break;
            case FuncK:
                if (tree->type == 1)
                    fprintf(listing, "Function: int %s\n", tree->attr.name);
                else
                    fprintf(listing, "Function: void %s\n", tree->attr.name);
                break;
            case CallK:
                fprintf(listing, "Call the Function: %s\n", tree->attr.name);
                break;
            case ParamsK:
                fprintf(listing, "Params\n");
                break;
            default:
                fprintf(listing, "Unknown ExpNode kind\n");
                break;
            }
        }
        else if (tree->nodekind == ExpK)
        {
            switch (tree->kind.exp)
            {
            case OpK:
                fprintf(listing, "Op: ");
                printToken(tree->attr.op, "\0");
                break;
            case ConstK:
                fprintf(listing, "Const: %d\n", tree->attr.val);
                break;
            case IdK:
                if (tree->is_vector)
                    fprintf(listing, "Vector: %s\n", tree->attr.name);
                else
                    fprintf(listing, "Id: %s\n", tree->attr.name);
                break;
            case TypeK:
                if (tree->type == 1)
                    fprintf(listing, "Type: Int\n");
                else
                    fprintf(listing, "Type: Void\n");
                break;
            case VectorK:
                fprintf(listing, "Vector: %s\n", tree->attr.name);
                break;
            default:
                fprintf(listing, "Unknown ExpNode kind\n");
                break;
            }
        }
        else
            fprintf(listing, "Unknown node kind\n");
        depth++;
        printTree(tree->firstchild);
        depth--;
        printTree(tree->nextsibling);
    }
}

/*************************************/

TreeNode *newStmtNode(StmtKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else
    {
        t->firstchild = NULL;
        t->nextsibling = NULL;
        t->nodekind = StmtK;
        t->kind.stmt = kind;
        t->lineno = lineno;
        t->scope = NULL;
        t->oper = NULL;
    }
    return t;
}

TreeNode *newExpNode(ExpKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else
    {
        t->firstchild = NULL;
        t->nextsibling = NULL;
        t->nodekind = ExpK;
        t->kind.exp = kind;
        t->lineno = lineno;
        t->type = Void;
        t->scope = NULL;
        t->oper = NULL;
    }
    return t;
}

char *copyString(char *s)
{
    int n;
    char *t;
    if (s == NULL)
        return NULL;
    n = strlen(s) + 1;
    t = malloc(n);
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else
        strcpy(t, s);
    return t;
}
