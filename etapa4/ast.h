#ifndef AST_HEADER
#define AST_HEADER

#include <stdlib.h>
#include "hash.h"
#include "symbols.h"

#define MAXSONS 4

typedef struct astnode
{
    int type;
    struct astnode *son[MAXSONS];
    HASH_NODE *symbol;
} AST;

AST *astCreate(int type, HASH_NODE *symbol, AST *son0, AST *son1, AST *son2, AST *son3);
void astPrint(AST *node, int level);

void generateSource(AST *node, FILE *out);
void closeFile(FILE *code);

#endif
