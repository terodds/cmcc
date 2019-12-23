#ifndef _SYNTREE_H_
#define _SYNTREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef int TokenType;
extern FILE *source;
extern FILE *code;

extern int lineno;

typedef enum {
    Variable,
    Array,
    TypeSpecifier
} NodeKind;

typedef enum {Void, Integer, Boolean} ExpType;

typedef struct TreeNode
{
    struct TreeNode* sibling;
    int lineno;
    NodeKind nodeKind;
    union {
        struct {
            struct TreeNode *type_specifier;
            struct TreeNode *_id;
        };
        
        struct {
            struct TreeNode *expression;
        };
             
        }attr;
} TreeNode;

extern int EchoSource;
extern int Error;
#endif
