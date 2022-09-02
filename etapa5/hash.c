// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#include "hash.h"
#include "ast.h"

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
    newNode->arguments = NULL;
    newNode->dataType = 0;
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
            printf("Table [%d] has %s dataType: %d type: %d\n", i, node->text, node->dataType, node->type);
        }
    }
}

int hashCheckUndeclared()
{
    HASH_NODE *node;
    int undeclared = 0;

    int i;

    for (i = 0; i < HASH_SIZE; i++)
    {
        for (node = table[i]; node; node = node->next)
        {
            if (node->type == SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "Semantic ERROR: Identifier '%s' wasn't declared.\n", node->text);
                ++undeclared;
            }
        }
    }

    return undeclared;
}