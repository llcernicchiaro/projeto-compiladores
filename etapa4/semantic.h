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
void checkBothOperators(AST *node);
int isNumber(AST *son);

#endif
