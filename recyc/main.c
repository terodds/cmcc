#include "globals.h"
#include "tree.h"

#define NO_PARSE 0


TreeNode *parse(void);


int lineno = 0;
FILE *source;
FILE *listing;
FILE *code;

int Error = 0;


int main(int argc, char *argv[])
{
    TreeNode *syntaxTree;
    char pgm[120];
    if(argc!=2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    strcpy(pgm, argv[1]);

    source = fopen(pgm, "r");
    if(source == NULL)
    {
        fprintf(stderr, "File %s not found\n", pgm);
        exit(1);
    }
    listing = stdout;
    #if NO_PARSE
        fprintf(listing, "  line number\t\ttoken\t\tlexeme\n");
        fprintf(listing, "----------------------------------\n");
        while(getToken() != ENDFILE);
    // 生成语法分析树
    #else
        syntaxTree = parse();
        fprintf(listing, "\nSyntax tree:\n");
       // printTree(syntaxTree);
        visitNode(syntaxTree);
    #endif

    // 分析表
    if(!Error)
    {
        fprintf(listing, "\nBuilding Symbol Table...\n");
        //buildSymtab(syntaxTree);
        fprintf(listing, "\nChecking Types...\n");
        //typeCheck(syntaxTree);
        fprintf(listing, "\nType Checking Finished\n");
    }

    // 生成中间代码
    if(! Error)
    {
        char *codefile;
        int fnlen = strcspn(pgm, ".");
        codefile = (char *)calloc(fnlen+4, sizeof(char));
        strncpy(codefile, pgm, fnlen);
        strcat(codefile, ".tm");
        code = fopen(codefile, "w");
        if(code == NULL)
        {
            printf("Unable to open %s\n", codefile);
            fclose(code);
        }
    }

    fclose(source);   
    return 0;
}
