%option noyywrap
%{
#include "globals.h"
#include "scan.h"
#include "util.h"
//#include "cm.tab.h"
//#include "token.h"
//#include "symtab.h"

char tokenString[MAXTOKENLEN+1];
static int yylex(void);
%}

digit   [0-9]
number  {digit}+
letter  [a-zA-Z]
id      {letter}+
newline \n
whitespace  [ \t]+

%%

"//".*                                    { /* DO NOTHING */ }
"#".*           { /*SKIP PREPROCESSING*/ }  
"if"            {return IF;}
"else"          {return ELSE;}
"int"           {return INT;}
"void"          {return VOID;}
"return"        {return RET;}
"!="            {return NEQ;}
"=="            {return EQ;}
"+"|"-"|"*"|"/"|"("|")"|"["|"]"|"{"|"}"|","|";"|"="            {return *yytext;}
"<="            {return LE;}
">="            {return GE;}
"<"             {return LT;}
">"             {return GE;}
{number}        {return NUM;}
{id}            {return ID;}    
{newline}       {lineno++;}
{whitespace}    {/*skip whitespace*/}
"/*"           {
                    char c,ant;
                    c = input();
                    do{
                        ant = c;
                        c = input();
                        if (c == EOF) break;
                        if (c == '\n') lineno++;
                    } while (ant != '*' || c!='/');
                }
.               {return ERROR;}


%%

/*
int main()
{
    int ret;
    while(ret=yylex())
    {
       printf("%d\n", ret); 
    }
    return 0;
}*/
/*
int main()
{
    Token tok = getToken();
    while(tok.type)
    {
       printToken(tok); 
       tok = getToken();
    }
    return 0;
}
*/
// void printToken(TokenType currentToken, const char *tokenString)
// {
//     printf("token<%d, %s>\n", currentToken, tokenString);
// }


TokenType getToken()
{   
    static int firstTime=TRUE;
    TokenType currentToken;
    if(firstTime){
        firstTime=FALSE;
        lineno++;
        yyin = source;
        yyout = listing;
    }
    currentToken = yylex();
    strncpy(tokenString, yytext, MAXTOKENLEN);
    if(TraceScan) {
        fprintf(listing, "\t%d: ", lineno);
        printToken(currentToken, tokenString);
    }
    return currentToken;
}
