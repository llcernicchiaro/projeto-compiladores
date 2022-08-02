// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#ifndef HASH_HEADER
#define HASH_HEADER

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define HASH_SIZE 997

typedef struct hash_node
{
    int type;
    char *text;
    struct hash_node *next;
} HASH_NODE;

void hashInit();
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint();

#endif
