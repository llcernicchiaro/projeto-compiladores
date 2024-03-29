// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#include "hash.h"
#include "ast.h"

void printVectorValuesASM(FILE *file, AST *node);

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
    newNode->value = (char *)calloc(strlen(text) + 1, sizeof(char));
    newNode->values = NULL;
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

HASH_NODE *makeLabel()
{
    static int serial = 0;
    char buffer[256];
    sprintf(buffer, "Weird__label%d", serial++);
    return hashInsert(buffer, SYMBOL_LABEL);
}

HASH_NODE *makeTemp(void)
{
    static int serial = 0;
    char buffer[256];
    sprintf(buffer, "mYWeeirdT_emp_%d", serial++);
    return hashInsert(buffer, SYMBOL_VARIABLE);
}

char *removeChars(char *s, char c)
{
    char *temp = s;
    int writer = 0, reader = 0;

    while (temp[reader])
    {
        if (temp[reader] != c)
        {
            temp[writer++] = temp[reader];
        }

        reader++;
    }

    temp[writer] = 0;

    return temp;
}

void hashPrintASM(FILE *fout)
{
    int i;
    HASH_NODE *node;

    fprintf(fout, "## DATA_SECTION\n.section\t.data\n");

    for (i = 0; i < HASH_SIZE; i++)
        for (node = table[i]; node; node = node->next)
        {
            switch (node->type)
            {
            case SYMBOL_STRING:
                fprintf(fout, "_%s:\n\t.string\t\"%s\"\n", removeChars(node->text, '"'), node->text);
                break;
            case SYMBOL_LITCHAR:
            case SYMBOL_LITINT:
            case SYMBOL_LITFLOAT:
                fprintf(fout, "_%s:\n\t.long\t%s\n", node->text, node->text);
                break;
            case SYMBOL_VARIABLE:
                fprintf(fout, "_%s:\n\t.long\t%s\n", node->text, node->value ? node->value : "0");
                break;
            case SYMBOL_VECTOR:
                if (node->values)
                {
                    fprintf(fout, "_%s:\n", node->text);
                    printVectorValuesASM(fout, node->values);
                }
                break;
            default:
                break;
            }
        }
}

void printVectorValuesASM(FILE *file, AST *node)
{
    if (node->son[0])
        fprintf(file, "\t.long\t%s\n", node->son[0]->symbol->text);
    if (node->son[1])
        if (node->son[1]->symbol)
            fprintf(file, "\t.long\t%s\n", node->son[1]->symbol->text);
    if (node->son[1])
        printVectorValuesASM(file, node->son[1]);
}