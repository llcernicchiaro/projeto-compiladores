// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#include <stdbool.h>
#include "semantic.h"

int semanticErrors = 0;

void checkAndSetDeclarations(AST *node)
{
    int i;

    if (node == 0)
        return;

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
        if (node->son[1])
        {
            node->symbol->value = node->son[1]->son[0]->symbol->text;
        }
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
        node->symbol->values = node->son[3]->son[0];

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

void checkGlobalVectorDeclaration(AST *node)
{

    int i;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_GLOBAL_VECTOR_DECLARATION:
        checkVectorOptionalInitialization(node);
        break;
    default:
        break;
    }

    for (i = 0; i < MAXSONS; ++i)
    {
        checkGlobalVectorDeclaration(node->son[i]);
    }
}

void checkVectorOptionalInitialization(AST *node)
{

    if (node->son[0] == 0 || node->son[1] == 0 || node->son[2] == 0 || node->symbol == 0)
        return;

    int vectorType = node->son[0]->type;
    char *vectorName = node->symbol->text;
    int vectorSize = atoi(node->son[1]->symbol->text);
    AST *currentVectorInitialization = node->son[2];

    int vectorCount = 0;

    while (currentVectorInitialization != 0)
    {
        switch (currentVectorInitialization->type)
        {
        case AST_OPTIONAL_INITIALIZATION:
            currentVectorInitialization = currentVectorInitialization->son[0];
            break;
        case AST_LITERAL_LIST:
            if (currentVectorInitialization->son[0])
            {
                if (!checkVectorInitializationCompatibleTypes(vectorType, currentVectorInitialization->son[0]->symbol))
                {
                    fprintf(stderr, "Semantic ERROR: vector '%s' initialization type missmatch\n", vectorName);
                    ++semanticErrors;
                }

                ++vectorCount;
            }
            currentVectorInitialization = currentVectorInitialization->son[1];
            break;
        case AST_LITERAL_LIST_TAIL:
            if (currentVectorInitialization->son[0])
            {
                if (!checkVectorInitializationCompatibleTypes(vectorType, currentVectorInitialization->son[0]->symbol))
                {
                    fprintf(stderr, "Semantic ERROR: vector '%s' initialization type missmatch\n", vectorName);
                    ++semanticErrors;
                }

                ++vectorCount;
            }
            currentVectorInitialization = currentVectorInitialization->son[1];
            break;
        default:
            break;
        }
    }

    if (vectorCount > vectorSize)
    {
        fprintf(stderr, "Semantic ERROR: vector '%s' initialization size missmatch - too many arguments \n", vectorName);
        ++semanticErrors;
    }
    else if (vectorCount < vectorSize)
    {
        fprintf(stderr, "Semantic ERROR: vector '%s' initialization size missmatch - too fewer arguments \n", vectorName);
        ++semanticErrors;
    }
}

void checkUndeclared()
{
    semanticErrors += hashCheckUndeclared();
}

void checkOperands(AST *node)
{
    int i;
    int result = 0;

    if (node == 0)
        return;

    for (i = 0; i < MAXSONS; i++)
        checkOperands(node->son[i]);

    switch (node->type)
    {
    case AST_ADD:
        result = checkBothOperands(node);
        break;
    case AST_SUB:
        result = checkBothOperands(node);
        break;
    case AST_MUL:
        result = checkBothOperands(node);
        break;
    case AST_DIV:
        result = checkBothOperands(node);
        break;
    case AST_GREATER_THAN:
        result = checkBothOperands(node);
        break;
    case AST_LESS_THAN:
        result = checkBothOperands(node);
        break;
    case AST_LE:
        result = checkBothOperands(node);
        break;
    case AST_GE:
        result = checkBothOperands(node);
        break;
    case AST_EQ:
        result = checkBothOperands(node);
        break;
    case AST_DIF:
        result = checkBothOperands(node);
        break;
    // case AST_ARRAY_ACCESS:
    //     if (!isNumber(node->son[0]))
    //     {
    //         fprintf(stderr, "Semantic ERROR: invalid vector index\n");
    //         ++semanticErrors;
    //     }
    //     result = 1;
    //     break;
    case AST_NOT:
        if (!isNumber(node->son[0]))
        {
            fprintf(stderr, "Semantic ERROR: invalid operator on NOT\n");
            ++semanticErrors;
        }
        result = 1;
        break;
    default:
        result = 1;
        break;
    }

    if (result == 0)
    {
        fprintf(stderr, "Semantic ERROR: operands are incompatible\n");
        ++semanticErrors;
    }
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

bool checkVectorInitializationCompatibleTypes(int type, HASH_NODE *symbol)
{
    bool isCharOrIntLit = symbol->type == SYMBOL_LITINT || symbol->type == SYMBOL_LITCHAR;

    switch (type)
    {
    case AST_INT:
    case AST_CHAR:
        if (isCharOrIntLit)
            return true;
        else
            return false;
    case AST_FLOAT:
        if (symbol->type == SYMBOL_LITFLOAT)
            return true;
        else
            return false;
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
        checkArgumentsSize((AST *)node->symbol->arguments, node);
        checkArgumentsTypes((AST *)node->symbol->arguments, node);
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

int checkBothOperands(AST *node)
{
    if (node == 0)
        return 0;

    if (node->type == AST_BRACKETS_EXPR)
        return checkBothOperands(node->son[0]);

    if (!node->son[1])
    {
        if (isFloat(node))
            return AST_FLOAT;
        else if (isNumber(node))
            return AST_INT;
        else
            return 0;
    }

    if (isFloat(node->son[0]) && isFloat(node->son[1]))
        return AST_FLOAT;
    else if (isNumber(node->son[0]) && isNumber(node->son[1]))
        return AST_INT;
    else
        return 0;
}

int isFloat(AST *son)
{
    bool isLitFloat = false;
    bool isTypeFloat = false;
    bool isValidVar = false;
    bool isValidParam = false;

    if (!son)
        return 0;

    if (son->symbol != 0)
    {
        isLitFloat = son->symbol->type == SYMBOL_LITFLOAT;
        isTypeFloat = son->symbol->dataType == DATA_TYPE_FLOAT;
        isValidVar = son->symbol->type == SYMBOL_VARIABLE && isTypeFloat;
        isValidParam = son->symbol->type == SYMBOL_PARAM && isTypeFloat;
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
         son->type == AST_NOT ||
         (son->type == AST_ARRAY_ACCESS && isTypeFloat) ||
         (son->type == AST_SYMBOL && (isLitFloat || isValidVar || isValidParam)) ||
         (son->type == AST_FUNC_CALL && isTypeFloat)))
        return 1;
    else
        return 0;
}

int isNumber(AST *son)
{
    bool isLitIntOrChar = false;
    bool isTypeIntOrChar = false;
    bool isValidVar = false;
    bool isValidParam = false;

    if (!son)
        return 0;

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
         son->type == AST_NOT ||
         (son->type == AST_ARRAY_ACCESS && isTypeIntOrChar) ||
         (son->type == AST_SYMBOL && (isLitIntOrChar || isValidVar || isValidParam)) ||
         (son->type == AST_FUNC_CALL && isTypeIntOrChar)))
        return 1;
    else
        return 0;
}

void checkAssignmentsType(AST *node)
{
    int i;

    if (node == 0)
        return;

    if (node->type == AST_ASSIGNMENT)
        matchAssignmentTypeWithExpressionType(node->symbol->dataType, node);

    for (i = 0; i < MAXSONS; ++i)
    {
        checkAssignmentsType(node->son[i]);
    }
}

void matchAssignmentTypeWithExpressionType(int assignmentDataType, AST *assignmentNode)
{
    if (!assignmentNode)
        return;

    if (assignmentNode->symbol->type == SYMBOL_VECTOR)
    {
        if ((assignmentDataType == DATA_TYPE_INT || assignmentDataType == DATA_TYPE_CHAR) && checkBothOperands(assignmentNode->son[1]) != AST_INT ||
            (assignmentDataType == DATA_TYPE_FLOAT && checkBothOperands(assignmentNode->son[1]) != AST_FLOAT))
        {
            fprintf(stderr, "Semantic ERROR: vector assignment type is invalid\n");
            ++semanticErrors;
        }
    }
    else if (assignmentNode->symbol->type == SYMBOL_VARIABLE)
    {
        if ((assignmentDataType == DATA_TYPE_INT || assignmentDataType == DATA_TYPE_CHAR) && checkBothOperands(assignmentNode->son[0]) != AST_INT ||
            (assignmentDataType == DATA_TYPE_FLOAT && checkBothOperands(assignmentNode->son[0]) != AST_FLOAT))
        {
            fprintf(stderr, "Semantic ERROR: variable assignment type is invalid\n");
            ++semanticErrors;
        }
    }
}

void checkFunctionsReturnType(AST *node)
{
    int i;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_FUNC_DEC:
        searchFunctionReturnFor(node);
        break;
    default:
        break;
    }

    for (i = 0; i < MAXSONS; ++i)
    {
        checkFunctionsReturnType(node->son[i]);
    }
}

void searchFunctionReturnFor(AST *node)
{
    if (node->son[0] == 0 || node->son[2] == 0 || node->symbol == 0)
        return;

    int functionType = node->son[0]->type;
    char *functionName = node->symbol->text;

    AST *cmdBlockPointer = node->son[2];

    while (cmdBlockPointer)
    {
        switch (cmdBlockPointer->type)
        {
        case AST_CMD_BLOCK:
            cmdBlockPointer = cmdBlockPointer->son[0];
            break;
        case AST_CMD_LIST:
        case AST_CMD_LIST_TAIL:
            if (cmdBlockPointer->son[0] != 0)
            {
                if (cmdBlockPointer->son[0]->type == AST_RETURN)
                {
                    matchReturnTypeWithFunctionType(functionType, cmdBlockPointer->son[0]);
                }
            }
            cmdBlockPointer = cmdBlockPointer->son[1];
            break;
        default:
            break;
        }
    }
}

void matchReturnTypeWithFunctionType(int functionType, AST *returnNode)
{
    if (!returnNode)
        return;

    if ((functionType == AST_INT || functionType == AST_CHAR) && checkBothOperands(returnNode->son[0]) != AST_INT ||
        (functionType == AST_FLOAT && checkBothOperands(returnNode->son[0]) != AST_FLOAT))
    {
        fprintf(stderr, "Semantic ERROR: return type is invalid\n");
        ++semanticErrors;
    }
}

void checkUseOf(AST *node)
{
    int i;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_ARRAY_ACCESS:
        if (node->symbol->type != SYMBOL_VECTOR)
        {
            fprintf(stderr, "Semantic ERROR: %s is not a vector\n", node->symbol->text);
            ++semanticErrors;
        }
        break;
    case AST_FUNC_CALL:
        if (node->symbol->type != SYMBOL_FUNCTION)
        {
            fprintf(stderr, "Semantic ERROR: %s is not a function\n", node->symbol->text);
            ++semanticErrors;
        }
        break;
    case AST_ASSIGNMENT:
        if (node->symbol)
        {
            if (node->symbol->type != SYMBOL_VARIABLE && !node->son[1])
            {
                fprintf(stderr, "Semantic ERROR: %s is not a variable\n", node->symbol->text);
                ++semanticErrors;
            }

            if (node->symbol->type != SYMBOL_VECTOR && node->son[1])
            {
                fprintf(stderr, "Semantic ERROR: %s is not a vector\n", node->symbol->text);
                ++semanticErrors;
            }

            if (node->symbol->type == SYMBOL_FUNCTION)
            {
                fprintf(stderr, "Semantic ERROR: functions are not assignable\n");
                ++semanticErrors;
            }
        }
        break;
    case AST_SYMBOL:
        if (node->symbol->type == SYMBOL_LITCHAR || node->symbol->type == SYMBOL_LITFLOAT ||
            node->symbol->type == SYMBOL_LITINT || node->symbol->type == SYMBOL_STRING)
            break;
        if (node->symbol->type != SYMBOL_VARIABLE && node->symbol->type != SYMBOL_PARAM)
        {
            fprintf(stderr, "Semantic ERROR: %s is not a variable\n", node->symbol->text);
            ++semanticErrors;
        }
        break;
    }

    for (i = 0; i < MAXSONS; ++i)
    {
        checkUseOf(node->son[i]);
    }
}