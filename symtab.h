#ifndef _SYMTAB_H_
# define _SYMTAB_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #include "../cminus.tab.h"

#define SYMTABLEN 100


typedef char string[16];
typedef enum {VAR, FUNC, ARR} Kind;
typedef int Type;

typedef struct Info
{
    Kind kind;
    Type type;
} Info;

typedef struct Record
{
    string name;
    Info *info;
} Record, Symbol;
//typedef Record Symbol;

typedef struct Symtab
{
    Record *rcdlist;
    int capcity;
    int nRecords;

} Symtab;


//extern Symtab symtab;

void initSymtab();
void destroySymtab();
void addName(string name);
int findName(string name);
Info *getInfo(string name);
void delRecord(string name);
#endif
