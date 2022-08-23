// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "hash.h"
#include "ast.h"

extern int semanticErrors;

void checkAndSetDeclarations(AST *node);
void checkUndeclared();
void checkOperands(AST *node);
void checkParameters(AST *node);

void checkArgumentsSize(AST *argsDeclared, AST *argsCall);
void setDataTypeWith(AST *node);
void checkBothOperators(AST *node);
int isNumber(AST *son);

#endif
