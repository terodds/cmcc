// #include "globals.h"
#include "util.h"

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */
void printToken(TokenType token, const char *tokenString)
{
    switch (token)
    {
    case IF:
    case ELSE:
    case INT:
    case WHILE:
    case RET:
    case VOID:
        fprintf(listing,
                "reserved word: %s\n", tokenString);
        break;
    case LT:
        fprintf(listing, "<\n");
        break;
    case LE:
        fprintf(listing, "<=\n");
        break;
    case GT:
        fprintf(listing, ">\n");
        break;
    case GE:
        fprintf(listing, ">=\n");
        break;
    case '=':
        fprintf(listing, "=\n");
        break;
    case NEQ:
        fprintf(listing, "!=\n");
        break;
    case EQ:
        fprintf(listing, "==\n");
        break;
    case '(':
        fprintf(listing, "(\n");
        break;
    case ')':
        fprintf(listing, ")\n");
        break;
    case '[':
        fprintf(listing, "[\n");
        break;
    case ']':
        fprintf(listing, "]\n");
        break;
    case '{':
        fprintf(listing, "{\n");
        break;
    case '}':
        fprintf(listing, "}\n");
        break;
    case ';':
        fprintf(listing, ";\n");
        break;
    case ',':
        fprintf(listing, ",\n");
        break;
    case '+':
        fprintf(listing, "+\n");
        break;
    case '-':
        fprintf(listing, "-\n");
        break;
    case '*':
        fprintf(listing, "*\n");
        break;
    case '/':
        fprintf(listing, "/\n");
        break;
    case ENDFILE:
        fprintf(listing, "EOF\n");
        break;
    case NUM:
        fprintf(listing,
                "NUM, val= %s\n", tokenString);
        break;
    case ID:
        fprintf(listing,
                "ID, name= %s\n", tokenString);
        break;
    case ERROR:
        fprintf(listing,
                "ERROR: %s\n", tokenString);
        break;
    default: /* should never happen */
        fprintf(listing, "Unknown token: %d\n", token);
    }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode *newStmtNode(StmtKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else
    {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = StmtK;
        t->kind.stmt = kind;
        t->lineno = lineno;
        t->scope = NULL;
        t->oper = NULL;
    }
    return t;
}

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode *newExpNode(ExpKind kind)
{
    TreeNode *t = (TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else
    {
        for (i = 0; i < MAXCHILDREN; i++)
            t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = ExpK;
        t->kind.exp = kind;
        t->lineno = lineno;
        t->type = Void;
        t->scope = NULL;
        t->oper = NULL;
    }
    return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
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

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno += 2
#define UNINDENT indentno -= 2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{
    int i;
    for (i = 0; i < indentno; i++)
        fprintf(listing, " ");
}

/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree(TreeNode *tree)
{
    int i;
    INDENT;
    while (tree != NULL)
    {
        printSpaces();
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
        for (i = 0; i < MAXCHILDREN; i++)
            printTree(tree->child[i]);
        tree = tree->sibling;
    }
    UNINDENT;
}

TreeNode *addSibling(TreeNode *origin, TreeNode *follow)
{
    if (origin != NULL)
    {
        TreeNode *t = origin;
        while (t->sibling != NULL)
            t = t->sibling;
        t->sibling = follow;
    }
    else
    {
        origin = follow;
    }
    return origin;
}


