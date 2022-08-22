// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "hash.h"
#include "ast.h"

extern int semanticErrors;

void checkAndSetDeclarations(AST *node);
void checkUndeclared();
void setDataTypeWith(AST *node);

#endif
