// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#include "hash.h"

HASH_NODE *table[HASH_SIZE];

void hashInit()
{
    int i;
    for (i = 0; i < HASH_SIZE; ++i)
    {
        table[i] = 0;
    }
}

int hashAddress(char *text)
{
    int address = 1;
    int i;

    for (i = 0; i < strlen(text); ++i)
    {
        address = (address * text[i]) % HASH_SIZE + 1;
    }

    return address - 1;
}

HASH_NODE *hashFind(char *text)
{
    HASH_NODE *node;
    int address = hashAddress(text);
    for (node = table[address]; node; node = node->next)
    {
        if (strcmp(node->text, text) == 0)
        {
            return node;
        }
    }

    return 0;
}

HASH_NODE *hashInsert(char *text, int type)
{
    HASH_NODE *newNode;

    int address = hashAddress(text);

    if ((newNode = hashFind(text)) != 0)
    {
        return newNode;
    }

    newNode = (HASH_NODE *)calloc(1, sizeof(HASH_NODE));
    newNode->type = type;
    newNode->text = (char *)calloc(strlen(text) + 1, sizeof(char));
    strcpy(newNode->text, text);
    newNode->next = table[address];
    table[address] = newNode;

    return newNode;
}

void hashPrint()
{
    int i;
    HASH_NODE *node;

    for (i = 0; i < HASH_SIZE; ++i)
    {
        for (node = table[i]; node; node = node->next)
        {
            printf("Table [%d] has %s\n", i, node->text);
        }
    }
}
