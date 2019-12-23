%{
#define YYPARSER

#include "globals.h"
#include "tree.h"
#include "scan.h"
#include "parse.h"

#define YYSTYPE TreeNode *

static TreeNode *savedTree;
static int yylex(void);
int yyerror(char *);

%}

/*%define api.value.type {Value} */

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
        $$ = mknode(DECL_L, addSibling($1, $2));
    }

    | declaration
    {   
        $$ = mknode(DECL_L, $1);
    }
    ;

declaration:
    var_declaration
    { $$ = mknode(DECL, $1); }
    | fun_declaration
    { $$ = mknode(DECL, $1); }
    ;

var_declaration:
    type_specifier _id ';'
    { $$ = mknode(VAR_DECL, $1); }
    | type_specifier _id '[' _num ']' ';'
    { $$ = mknode(VAR_DECL, addSibling($1, $3)); }
    ;

type_specifier:
    INT
    { $$ = mknode(TYPE, mknode(INT, NULL)); }
    | VOID
    { $$ = mknode(TYPE, mknode(VOID, NULL)); }
    ;

fun_declaration:
    type_specifier _id '(' params ')' 
    { $$ = mknode(FUN_DECL, addSibling($1, addSibling($2,$4))); }
    | type_specifier _id '(' params ')' compound_stmt
    { $$ = mknode(FUN_DECL, addSibling($1, $2));
      addSibling($2, addSibling($4, $6)); }
    ;

params:
    param_list
    { $$ = mknode(PARAM, $1); }
    | VOID  
    { $$ = mknode(PARAM, mknode(VOID, NULL)); }
    ;

param_list:
    param_list ',' param
    { $$ = addSibling($1, $2); }
    | param
    { $$ = $1; }
    ;

param:
    type_specifier _id
    { $$ = mknode(PARAM_VAR, addSibling($1, $2)); }
    |  type_specifier _id '[' ']'
    { 
      $$ = mknode(PARAM_ARR, addSibling($1, $2));
    }
    ;

compound_stmt:
    '{' local_declarations statement_list '}'
    { $$ = mknode(CMPD_STMT, addSibling($2, $3)); }
    ;

local_declarations:
    local_declarations var_declaration 
    { $$ = addSibling($1, $2); }
    | %empty
    { $$ = mknode(EMPTY, NULL); }
    ;

statement_list: 
    statement_list statement
    { $$ = addSibling($1, $2); }
    | %empty 
    { $$ = NULL; }
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
    { $$ = mknode(EXPR_STMT, $1); }
    | ';'
    { $$ = NULL; }
    ;

selection_stmt:
    IF '(' expression ')' statement %prec LOWER_ELSE
    { $$ = mknode(SELECT_STMT, addSibling($3,$5)); }
    | IF '(' expression  ')' statement ELSE statement
    { $$ = mknode(SELECT_STMT, addSibling($3, addSibling($5, $7))); }
    ;

iteration_stmt:
    WHILE '(' expression ')' statement
    { $$ = mknode(ITER_STMT, addSibling($3, $5)); }
    ;

return_stmt:
    RET ';'
    { $$ = mknode(RET_STMT, NULL); }
    | RET expression    
    { $$ = mknode(RET_STMT, $1); }
    ;

expression:
    var '=' simple_expression
    { $$ = mknode(ASSIGN_EXPR, addSibling($1, $3)); }
    | simple_expression
    { $$ = $1; }
    ;

var:
    _id %prec ID 
    { $$ = $1; }
    | _id '[' expression ']'
    { $$ = mknode(ARRAY_EL, addSibling($1, $3)); }
    ;

simple_expression:
    additive_expression relop additive_expression
    { $$ = mknode(REL_EXPR, addSibling($1, addSibling($2, $3))); }
    | additive_expression
    { $$ = $1; }
    ;

relop:
    LE 
    { $$ = mknode(LE, NULL); }
    | LT 
    { $$ = mknode(LT, NULL); }
    | GT 
    { $$ = mknode(GT, NULL); }
    | GE 
    { $$ = mknode(GE, NULL); }
    | EQ
    { $$ = mknode(EQ, NULL); } 
    | NEQ
    { $$ = mknode(NEQ, NULL); }
    ;

additive_expression:
    additive_expression addop term
    { $$ = mknode(ADD_EXPR, addSibling($1, addSibling($2, $3))); }
    | term
    { $$ = $1; }
    ;

addop:
    '+' 
    { $$ = mknode('+', NULL); }
    | '-'
    { $$ = mknode('-', NULL); }
    ;

term:
    term mulop factor
    { $$ = mknode(MUL_EXPR, addSibling($1, addSibling($2, $3))); }
    | factor
    { $$ = $1; }
    ;

mulop:
    '*'
    { $$ = mknode('*', NULL); } 
    | '/'
    { $$ = mknode('/', NULL); }
    ;

factor:
    '(' expression ')'
    { $$ = $1; }
    | var
    { $$ = $1; }
    | call
    { $$ = $1; }
    | _num
    { $$ = $1; }
    ;

call:
    _id '(' args ')' 
    { $$ = mknode(CALL, addSibling($1, $3)); }
   ;

args:
    arg_list
    { $$ = $1; }
    | %empty 
    { $$ = NULL; }
    ;

arg_list:
    arg_list ',' expression
    { $$ = addSibling($1, $3); }
    | expression
    { $$ = $1; }
    ;
  
_id:
    ID
    { $$ = mknode(ID, NULL); }
    ;

_num:
    NUM
    { $$ = mknode(NUM, NULL); }
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
