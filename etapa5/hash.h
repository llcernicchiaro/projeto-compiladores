// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#ifndef HASH_HEADER
#define HASH_HEADER

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define HASH_SIZE 997

#define SYMBOL_IDENTIFIER 1
#define SYMBOL_LITINT 2
#define SYMBOL_LITFLOAT 3
#define SYMBOL_LITCHAR 4
#define SYMBOL_STRING 5
#define SYMBOL_VARIABLE 6
#define SYMBOL_FUNCTION 7
#define SYMBOL_VECTOR 8
#define SYMBOL_PARAM 9

#define DATA_TYPE_INT 1
#define DATA_TYPE_FLOAT 2
#define DATA_TYPE_CHAR 3

typedef struct hash_node
{
    int type;
    int dataType;
    char *text;
    struct AST *arguments;
    struct hash_node *next;
} HASH_NODE;

void hashInit();
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint();
int hashCheckUndeclared();

#endif
