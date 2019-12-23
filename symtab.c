#include "symtab.h"
/**
 * Class Symtab
 */


static Symtab symtab;

void initSymtab()
{
    symtab.capcity = SYMTABLEN;
    symtab.rcdlist = (Record *)malloc(sizeof(Record)*symtab.capcity);
    symtab.nRecords = 0;
}

void destroySymtab()
{
    free(symtab.rcdlist);
}

void addName(string name)
{   
    Record *p = symtab.rcdlist + (symtab.nRecords++);
    //p->name = name
    strcpy(p->name, name);
}

int findName(string name)
{
    int i=symtab.nRecords-1;
    //while(i>=0 && rcdlist[i].getName() != name)
    while(i>=0 && strcmp(symtab.rcdlist[i].name, name)!=0)
    {
        i--;
    }
    return i; 
}

Info *getInfo(string name)
{
    int i = findName(name);
    if(i<0) return NULL;
    else return symtab.rcdlist[i].info;
}

void delRecord(string name)
{
    int pos = findName(name);
    int i;
    if(pos<0) return;
    for(i=pos; i<symtab.nRecords-1; i++) 
    {
        symtab.rcdlist[i] = symtab.rcdlist[i+1];   
    }
}

void updateInfo()
{

}
