%{
#define YYPARSER

#include "globals.h"
// #include "tree.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode *

static TreeNode *savedTree;
static int yylex(void);
int yyerror(char *);

%}


%token ENDFILE ERROR
%token INT VOID
%left ID NUM
%left '('
%nonassoc LT LE GT GE EQ NEQ
%token IF WHILE RET
%nonassoc LOWER_ELSE
%nonassoc ELSE

%% 

program:
    declaration_list
    {savedTree = $1;}
    ;

declaration_list : 
    declaration_list declaration
    {
        $$ = addSibling($1, $2);
    }
    | declaration
    {   
        $$ = $1;
    }
    ;

declaration:
    var_declaration
    { $$ = $1; }
    | fun_declaration
    { $$ = $1; }
    ;

var_declaration:
    type_specifier _id ';'
    { 
        $$ = $1;
        $$->child[0] = $2;
        // addChild($$, $2);
    }
    | type_specifier _id '[' _num ']' ';'
    { 
        $$ = $1;
        $2->is_vector = 1;
        $$->child[0] = $2;
        $2->child[0] = $2;
        // addChild($$, $2);
        // addChild($2, $4);
    }
    ;

type_specifier:
    INT
    { 
        $$ = newExpNode(TypeK);
        $$->type = Integer;
        $$->attr.name = copyString(tokenString);
    }
    | VOID
    { 
        $$ = newExpNode(TypeK);
        $$->type = Void;
        $$->attr.name = copyString(tokenString);
    }
    ;

fun_declaration:
    type_specifier _id '(' params ')'
    { 
        $$ = newStmtNode(FuncK);
        $$->attr.name = $2->attr.name;
        // addChild($$, $4);
        $$->child[0] = $4;
    }
    | type_specifier _id '(' params ')' compound_stmt
    { 
        $$ = newStmtNode(FuncK);
        $$->attr.name = $2->attr.name;
        // addChild($$, $4);
        // addSibling($4, $6);
        $$->child[0] = $4;
        $$->child[1] = $6;
        if (!strcmp($1->attr.name, "int"))
            $$->type = Integer;
        else
            $$->type = Void;
    }
    ;

params:
    param_list
    { 
        $$ = newStmtNode(ParamsK);
        // $$->firstchild = $1;
        // addChild($$, $1);
        $$->child[0] = $1;
    }
    | VOID
    { $$ = NULL; }
    ;

param_list:
    param_list ',' param
    { 
      $$ = addSibling($1, $3);
    }
    | param
    { $$ = $1; }
    ;

param:
    type_specifier _id
    { 
      $$ = $1;
    //   addChild($$, $2);
      $$->child[0] = $2;
      $2->is_vector = 0;
    }
    |  type_specifier _id '[' ']'
    { 
        $$ = $1;
        // addChild($$, $2);
        $$->child[0] = $2;
        $2->is_vector = 1;
    }
    ;

compound_stmt:
    '{' local_declarations statement_list '}'
    { $$ = addSibling($2, $3); }
    ;

local_declarations:
    local_declarations var_declaration 
    { 
        $$ = addSibling($1, $2);   
    }
    | %empty
    { $$ = NULL; }
    ;

statement_list: 
    statement_list statement
    { 
        $$ = addSibling($1, $2);
    }
    | %empty 
    {
        $$ = NULL; 
    }
    ;

statement:
    expression_stmt
    { $$ = $1; }
    | compound_stmt
    { $$ = $1; }
    | selection_stmt
    { $$ = $1; }
    | iteration_stmt
    { $$ = $1; }
    | return_stmt
    { $$ = $1; }
    ;

expression_stmt:
    expression ';'
    { $$ = $1; }
    | ';'
    { $$ = NULL; }
    ;

selection_stmt:
    IF '(' expression ')' statement %prec LOWER_ELSE
    { 
        $$ = newStmtNode(IfK);
        // addChild($$, $3);
        // addSibling($3, $5);
         $$->child[0] = $3;
         $$->child[1] = $5;
    }
    | IF '(' expression  ')' statement ELSE statement
    { 
        $$ = newStmtNode(IfK);
        $$->child[0] = $3;
        $$->child[1] = $5;
        $$->child[2] = $7;
        // addChild($$, $3);
        // addSibling($3, $5);
        // addSibling($5, $7);
    }
    ;

iteration_stmt:
    WHILE '(' expression ')' statement
    { 
        $$ = newStmtNode(WhileK);
        // addChild($$, $3);
        // addSibling($3, $5);
        $$->child[0] = $3;
        $$->child[1] = $5;
    }
    ;

return_stmt:
    RET ';'
    { $$ = newStmtNode(ReturnK); }
    | RET expression    
    { 
        $$ = newStmtNode(ReturnK);
        // addChild($$, $2);
        $$->child[0] = $2;
    }
    ;

expression:
    var '=' simple_expression
    {
        $$ = newStmtNode(AssignK);
        // addChild($$, $1);
        // addSibling($1, $3);
        $$->child[0] = $1;
        $$->child[1] = $3;
    }
    | simple_expression
    { $$ = $1; }
    ;

var:
    _id %prec ID 
    { 
        $$ = $1;
        $1->is_vector = 0;
    }
    | _id '[' expression ']'
    {
        $$ = newExpNode(VectorK);
        $$->attr.name = $1->attr.name;
        $$->is_vector = 1;
        // addChild($$, $3);
        $$->child[0] = $3;
    }
    ;

simple_expression:
    additive_expression relop additive_expression
    { 
        $$ = newExpNode(OpK);
        $$->attr.op = $2->attr.op;
        // addChild($$, $1);
        // addSibling($1, $3);
        $$->child[0] = $1;
        $$->child[1] = $3;
    }
    | additive_expression
    { $$ = $1; }
    ;

relop:
    LE 
    { $$ = newExpNode(OpK); $$->attr.op = LE;}
    | LT 
    { $$ = newExpNode(OpK); $$->attr.op = LT; }
    | GT 
    { $$ = newExpNode(OpK); $$->attr.op = GT; }
    | GE 
    { $$ = newExpNode(OpK); $$->attr.op = GE; }
    | EQ
    { $$ = newExpNode(OpK); $$->attr.op = EQ; } 
    | NEQ
    { $$ = newExpNode(OpK); $$->attr.op = NEQ; }
    ;

additive_expression:
    additive_expression addop term
    { 
        $$ = newExpNode(OpK);
        $$->attr.op = $2->attr.op;
        // $$ = $2;
        // addChild($$, $1);
        // addSibling($1, $3);
        $$->child[0] = $1;
        $$->child[1] = $3;
    }
    | term
    { $$ = $1; }
    ;

addop:
    '+' 
    { $$ = newExpNode(OpK); $$->attr.op = '+'; }
    | '-'
    { $$ = newExpNode(OpK); $$->attr.op = '-'; }
    ;

term:
    term mulop factor
    {
      $$ = newExpNode(OpK);
    //   $$ = $2;
    //   addChild($$, $1);
    //   addSibling($1, $3);
     $$->child[0] = $1;
     $$->child[1] = $3;
      $$->attr.op = $2->attr.op;
    }
    | factor
    { $$ = $1; }
    ;

mulop:
    '*'
    { $$ = newExpNode(OpK); $$->attr.op = '*'; } 
    | '/'
    { $$ = newExpNode(OpK); $$->attr.op = '/'; }
    ;

factor:
    '(' expression ')'
    { $$ = $2; }
    | var
    { $$ = $1; }
    | call
    { $$ = $1; }
    | _num
    { $$ = $1; }
    ;

call:
    _id '(' args ')' 
    { 
        $$ = newStmtNode(CallK);
        $$->attr.name = $1->attr.name;
        // addChild($$, $3);
        $$->child[0] = $3;
    }
   ;

args:
    arg_list
    { $$ = $1; }
    | %empty
    { $$ = NULL; }
    ;

arg_list:
    arg_list ',' expression
    { 
        $$ = addSibling($1, $3); 
    }
    | expression
    { $$ = $1; }
    ;
  
_id:
    ID
    { 
        $$ = newExpNode(IdK); 
        $$->attr.name = copyString(tokenString);
    }
    ;

_num:
    NUM
    { 
        $$ = newExpNode(ConstK);
        $$->attr.val = atoi(copyString(tokenString));
    }
    ;

%%


int yyerror(char *s)
{
    fprintf(listing, "Syntax error at line %d: %s\n", lineno, s);
    fprintf(listing, "Current token: ");
    printToken(yychar, tokenString);
    Error = TRUE;
    return 0;
}


static int yylex(void)
{
    return getToken();
}


TreeNode * parse(void)
{
    yyparse();
    return savedTree;
}
