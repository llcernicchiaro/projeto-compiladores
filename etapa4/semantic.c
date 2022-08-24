// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#include <stdbool.h>
#include "semantic.h"

int semanticErrors = 0;

void checkAndSetDeclarations(AST *node)
{
    int i;

    if (node == 0)
    {
        return;
    }

    switch (node->type)
    {
    case AST_GLOBAL_VARIABLE_DECLARATION:
        if (node->symbol)
        {
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                if (node->symbol->text)
                {
                    fprintf(stderr, "Semantic ERROR: variable %s already declared\n", node->symbol->text);
                    ++semanticErrors;
                }
            }
        }

        node->symbol->type = SYMBOL_VARIABLE;

        setDataTypeWith(node);

        break;

    case AST_FUNC_DEC:
        if (node->symbol)
        {
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                if (node->symbol->text)
                {
                    fprintf(stderr, "Semantic ERROR: function %s already declared\n", node->symbol->text);
                    ++semanticErrors;
                }
            }
        }

        node->symbol->arguments = node->son[1];
        node->symbol->type = SYMBOL_FUNCTION;

        setDataTypeWith(node);
        break;

    case AST_GLOBAL_VECTOR_DECLARATION:
        if (node->symbol)
        {
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                if (node->symbol->text)
                {
                    fprintf(stderr, "Semantic ERROR: vector %s already declared\n", node->symbol->text);
                    ++semanticErrors;
                }
            }
        }

        node->symbol->type = SYMBOL_VECTOR;

        setDataTypeWith(node);

        break;

    case AST_PARAM:
        node->symbol->type = SYMBOL_PARAM;
        setDataTypeWith(node);
        break;
    }

    for (i = 0; i < MAXSONS; ++i)
    {
        checkAndSetDeclarations(node->son[i]);
    }
}

void checkUndeclared()
{
    semanticErrors += hashCheckUndeclared();
}

void checkOperands(AST *node)
{
    int i;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_ADD:
        checkBothOperators(node);
        break;
    case AST_SUB:
        checkBothOperators(node);
        break;
    case AST_MUL:
        checkBothOperators(node);
        break;
    case AST_DIV:
        checkBothOperators(node);
        break;
    case AST_GREATER_THAN:
        checkBothOperators(node);
        break;
    case AST_LESS_THAN:
        checkBothOperators(node);
        break;
    case AST_LE:
        checkBothOperators(node);
        break;
    case AST_GE:
        checkBothOperators(node);
        break;
    case AST_EQ:
        checkBothOperators(node);
        break;
    case AST_DIF:
        checkBothOperators(node);
        break;
    case AST_BRACKETS_EXPR:
        if (!isNumber(node->son[0]))
        {
            fprintf(stderr, "Semantic ERROR: invalid expression inside brackets\n");
            ++semanticErrors;
        }
        break;
    case AST_ARRAY_ACCESS:
        if (!isNumber(node->son[0]))
        {
            fprintf(stderr, "Semantic ERROR: invalid vector index\n");
            ++semanticErrors;
        }
        break;
    case AST_NOT:
        if (!isNumber(node->son[0]))
        {
            fprintf(stderr, "Semantic ERROR: invalid operator on NOT\n");
            ++semanticErrors;
        }
        break;
    default:
        break;
    }

    for (i = 0; i < MAXSONS; i++)
        checkOperands(node->son[i]);
}

void checkArgumentsSize(AST *argsDeclared, AST *argsCall)
{
    AST *actualDeclared = NULL;
    AST *actualCall = NULL;
    int declaredLength = 0;
    int callLength = 0;

    if (argsDeclared)
        actualDeclared = argsDeclared;

    if (argsCall)
        actualCall = argsCall->son[0];

    while (actualDeclared != NULL)
    {
        declaredLength += 1;
        actualDeclared = actualDeclared->son[1];
    }

    while (actualCall != NULL)
    {
        callLength += 1;
        actualCall = actualCall->son[1];
    }

    if (declaredLength < callLength)
    {
        fprintf(stderr, "Semantic ERROR: too many arguments in function call\n");
        ++semanticErrors;
    }
    else if (callLength < declaredLength)
    {
        fprintf(stderr, "Semantic ERROR: too fewer arguments in function call\n");
        ++semanticErrors;
    }
}

bool checkCompatibleTypes(int type, HASH_NODE *symbol)
{
    bool isVar = symbol->type == 6;
    bool isCharOrIntVar = isVar && symbol->dataType == DATA_TYPE_INT || symbol->dataType == DATA_TYPE_CHAR;
    bool isCharOrIntLit = symbol->type == SYMBOL_LITINT || symbol->type == SYMBOL_LITCHAR;

    switch (type)
    {
    case DATA_TYPE_INT:
    case DATA_TYPE_CHAR:
        if (isCharOrIntVar || isCharOrIntLit)
            return true;
        else
            return false;
    case DATA_TYPE_FLOAT:
        if ((isVar && symbol->dataType == DATA_TYPE_FLOAT) || symbol->type == SYMBOL_LITFLOAT)
            return true;
        else
            return false;
    }
}

void checkArgumentsTypes(AST *argsDeclared, AST *argsCall)
{
    AST *actualDeclared = NULL;
    AST *actualCall = NULL;
    int argIndex = 0;

    if (argsDeclared)
        actualDeclared = argsDeclared;

    if (argsCall)
        actualCall = argsCall->son[0];

    while (actualDeclared != NULL && actualCall != NULL)
    {
        if (!checkCompatibleTypes(actualDeclared->son[0]->symbol->dataType, actualCall->son[0]->symbol))
        {
            fprintf(stderr, "Semantic ERROR: argument %d with wrong type in function call\n", argIndex);
            ++semanticErrors;
        }
        argIndex += 1;
        actualDeclared = actualDeclared->son[1];
        actualCall = actualCall->son[1];
    }
}

void checkParameters(AST *node)
{
    int i;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_FUNC_CALL:
        checkArgumentsSize(node->symbol->arguments, node);
        checkArgumentsTypes(node->symbol->arguments, node);
        break;
    }

    for (i = 0; i < MAXSONS; ++i)
    {
        checkParameters(node->son[i]);
    }
}

void setDataTypeWith(AST *node)
{
    if (node->son[0])
    {
        switch (node->son[0]->type)
        {
        case AST_INT:
            node->symbol->dataType = DATA_TYPE_INT;
            break;
        case AST_FLOAT:
            node->symbol->dataType = DATA_TYPE_FLOAT;
            break;
        case AST_CHAR:
            node->symbol->dataType = DATA_TYPE_CHAR;
            break;
        }
    }
}

void checkBothOperators(AST *node)
{

    if (node == 0)
        return;

    if (node->son[0])
        if (!isNumber(node->son[0]))
        {
            fprintf(stderr, "Semantic ERROR: invalid left operand\n");
            ++semanticErrors;
        }

    if (node->son[1])
        if (!isNumber(node->son[1]))
        {
            fprintf(stderr, "Semantic ERROR: invalid right operand\n");
            ++semanticErrors;
        }
}

int isNumber(AST *son)
{
    bool isLitIntOrChar = false;
    bool isTypeIntOrChar = false;
    bool isValidVar = false;
    bool isValidParam = false;
    if (son->symbol != 0)
    {
        isLitIntOrChar = son->symbol->type == SYMBOL_LITINT || son->symbol->type == SYMBOL_LITCHAR;
        isTypeIntOrChar = son->symbol->dataType == DATA_TYPE_INT || son->symbol->dataType == DATA_TYPE_CHAR;
        isValidVar = son->symbol->type == SYMBOL_VARIABLE && isTypeIntOrChar;
        isValidParam = son->symbol->type == SYMBOL_PARAM && isTypeIntOrChar;
    }

    if ((son->type == AST_ADD ||
         son->type == AST_SUB ||
         son->type == AST_MUL ||
         son->type == AST_DIV ||
         son->type == AST_GREATER_THAN ||
         son->type == AST_LESS_THAN ||
         son->type == AST_LE ||
         son->type == AST_GE ||
         son->type == AST_EQ ||
         son->type == AST_DIF ||
         son->type == AST_BRACKETS_EXPR ||
         son->type == AST_ARRAY_ACCESS ||
         son->type == AST_NOT ||
         (son->type == AST_SYMBOL && (isLitIntOrChar || isValidVar || isValidParam)) ||
         (son->type == AST_FUNC_CALL && isTypeIntOrChar)))
        return 1;
    else
        return 0;
}
