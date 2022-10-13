// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#ifndef TAC_HEADER
#define TAC_HEADER

#include <stdio.h>
#include "symbols.h"
#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MULT 4
#define TAC_DIV 5
#define TAC_COPY 6
#define TAC_LABEL 7
#define TAC_BEGIN_FUN 8
#define TAC_END_FUN 9
#define TAC_IFZ 10
#define TAC_JUMP 11
#define TAC_CALL 12
#define TAC_ARG 13
#define TAC_RETURN 14
#define TAC_PRINT 15
#define TAC_READ 16
#define TAC_LESS 17
#define TAC_GREATER 18
#define TAC_OR 19
#define TAC_AND 20
#define TAC_LE 21
#define TAC_GE 22
#define TAC_EQ 23
#define TAC_DIF 24
#define TAC_FUNC_CALL 25
#define TAC_JFALSE 26
#define TAC_IF_ELSE 27
#define TAC_VEC_DECLARATION 28
#define TAC_VAR_DECLARATION 29
#define TAC_PARAMS_FUNC_CALL 30

typedef struct tac_node
{
    int type;
    HASH_NODE *res;
    HASH_NODE *op1;
    HASH_NODE *op2;
    struct tac_node *prev;
    struct tac_node *next;
} TAC;

TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);
void tacPrintSingle(TAC *tac);
void tacPrintBack(TAC *tac);
TAC *tacGenerateCode(AST *node);
TAC *tacJoin(TAC *t1, TAC *t2);
TAC *makeParameter(TAC *code1, TAC *code2);
TAC *makeFunc(AST *symbol, TAC *code1, TAC *code2);
TAC *makeIfThen(TAC *expr, TAC *body);
TAC *makeWhile(TAC *expr, TAC *body);
TAC *makeIfThenElse(TAC *expr, TAC *cIf, TAC *cElse);
TAC *makeCondJump(TAC *label, TAC *expr);
TAC *makeIncondJump(TAC *label);

#endif