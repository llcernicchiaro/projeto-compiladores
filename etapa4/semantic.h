// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include <stdbool.h>
#include "hash.h"
#include "ast.h"

extern int semanticErrors;

void checkAndSetDeclarations(AST *node);
void checkUndeclared();
void checkOperands(AST *node);
void checkParameters(AST *node);

bool checkCompatibleTypes(int type, HASH_NODE *symbol);
void checkArgumentsTypes(AST *argsDeclared, AST *functionCall);
void checkArgumentsSize(AST *argsDeclared, AST *functionCall);
void setDataTypeWith(AST *node);
int checkBothOperands(AST *node);
int isNumber(AST *son);
void checkGlobalVectorDeclaration(AST *node);
void checkVectorOptionalInitialization(AST *node);
bool checkVectorInitializationCompatiableTypes(int type, HASH_NODE *symbol);
void checkFunctionsReturnType(AST *node);
void searchFunctionReturnFor(AST *node);
void matchReturnTypeWithFunctionType(int functionType, AST* returnNode);
bool checkVectorInitializationCompatibleTypes(int type, HASH_NODE *symbol);
int isFloat(AST *son);
void checkAssignmentsType(AST *node);
void matchAssignmentTypeWithExpressionType(int assignmentDataType, AST* assignmentNode);
void checkUseOf(AST* node);

#endif
