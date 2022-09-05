// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#include "tac.h"

TAC* makeFunc(AST* symbol, TAC* code1, TAC* code2);
TAC *makeIfThen(TAC *expr, TAC *body);
TAC *makeWhile(TAC *expr, TAC *body);
TAC* makeIfThenElse(TAC* expr, TAC* cIf, TAC* cElse);
TAC* makeCondJump(TAC* label, TAC* expr);
TAC* makeIncondJump(TAC* label);

TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2)
{
    TAC *newTAC;

    newTAC = (TAC *)calloc(1, sizeof(TAC));
    newTAC->type = type;
    newTAC->res = res;
    newTAC->op1 = op1;
    newTAC->op2 = op2;
    newTAC->next = 0;
    newTAC->prev = 0;

    return newTAC;
}

void tacPrintSingle(TAC *tac)
{
    if (!tac)
        return;

    if (tac->type == TAC_SYMBOL)
        return;

    fprintf(stderr, "TAC ( ");

    switch (tac->type)
    {
    case TAC_SYMBOL:
        fprintf(stderr, "TAC_SYMBOL");
        break;

    case TAC_ADD:
        fprintf(stderr, "TAC_ADD");
        break;

    case TAC_SUB:
        fprintf(stderr, "TAC_SUB");
        break;

    case TAC_MULT:
        fprintf(stderr, "TAC_MULT");
        break;

    case TAC_DIV:
        fprintf(stderr, "TAC_DIV");
        break;

    case TAC_LESS:
        fprintf(stderr, "TAC_LESS");
        break;

    case TAC_GREATER:
        fprintf(stderr, "TAC_GREATER");
        break;

    case TAC_OR:
        fprintf(stderr, "TAC_OR");
        break;

    case TAC_AND:
        fprintf(stderr, "TAC_AND");
        break;

    case TAC_LE:
        fprintf(stderr, "TAC_LE");
        break;

    case TAC_GE:
        fprintf(stderr, "TAC_GE");
        break;

    case TAC_EQ:
        fprintf(stderr, "TAC_EQ");
        break;

    case TAC_DIF:
        fprintf(stderr, "TAC_DIF");
        break;

    case TAC_READ:
        fprintf(stderr, "TAC_READ");
        break;

    case TAC_PRINT:
        fprintf(stderr, "TAC_PRINT");
        break;

    case TAC_RETURN:
        fprintf(stderr, "TAC_RETURN");
        break;

    case TAC_COPY:
        fprintf(stderr, "TAC_COPY");
        break;

    case TAC_JFALSE:
        fprintf(stderr, "TAC_JFALSE");
        break;

    case TAC_LABEL:
        fprintf(stderr, "TAC_LABEL");
        break;

    case TAC_JUMP:
        fprintf(stderr, "TAC_JUMP");
        break;

    case TAC_BEGIN_FUN:
        fprintf(stderr, "TAC_BEGIN_FUNC");
        break;

    case TAC_END_FUN:
        fprintf(stderr, "TAC_END_FUNC");
        break;

    case TAC_FUNC_CALL:
        fprintf(stderr, "TAC_FUNC_CALL");
        break;

    case TAC_IF_ELSE:
        fprintf(stderr, "TAC_IF_ELSE");
        break;

    case TAC_VEC_DECLARATION:
        fprintf(stderr, "TAC_VEC_DECLARATION");
        break;

    case TAC_VAR_DECLARATION:
        fprintf(stderr, "TAC_VAR_DECLARATION");
        break;

    default:
        fprintf(stderr, "TAC_UNKNOWN - %d", tac->type);
        break;
    }

    fprintf(stderr, ", %s", tac->res->text);
    fprintf(stderr, ",%s", (tac->op1) ? tac->op1->text : "0");
    fprintf(stderr, ",%s", (tac->op2) ? tac->op2->text : "0");
    fprintf(stderr, ");\n");
}

void tacPrintBack(TAC *tac)
{
    TAC *node;

    for (node = tac; node; node = node->prev)
        tacPrintSingle(node);
}

TAC *tacJoin(TAC *tac1, TAC *tac2)
{
    if (!tac1)
        return tac2;

    if (!tac2)
        return tac1;

    TAC *aux;
    for (aux = tac2; aux->prev != 0; aux = aux->prev)
        ;
    aux->prev = tac1;
    return tac2;
}

TAC* makeBinaryOperation(TAC* code0, TAC* code1, int tactype)
{
    return tacJoin(tacJoin(code0, code1), tacCreate(tactype, makeTemp(), code0?code0->res:0, code1?code1->res:0));
}

TAC *tacGenerateCode(AST *node)
{
    int i;
    TAC *result = 0;
    TAC *code[MAXSONS];

    if (!node)
        return 0;

    for (i = 0; i < MAXSONS; i++)
    {
        code[i] = tacGenerateCode(node->son[i]);
    }

    switch (node->type)
    {
    case AST_SYMBOL:
        result = tacCreate(TAC_SYMBOL, node->symbol, 0, 0);
        break;
    case AST_ADD:
        result = makeBinaryOperation(code[0], code[1], TAC_ADD);
        break;
    case AST_SUB:
        result = makeBinaryOperation(code[0], code[1], TAC_SUB);
        break;
    case AST_MUL:
        result = makeBinaryOperation(code[0], code[1], TAC_MULT);
        break;
    case AST_DIV:
        result = makeBinaryOperation(code[0], code[1], TAC_DIV);
        break;
    case AST_LESS_THAN:
        result = makeBinaryOperation(code[0], code[1], TAC_LESS);
        break;
    case AST_GREATER_THAN:
        result = makeBinaryOperation(code[0], code[1], TAC_GREATER);
        break;
    case AST_LE:
        result = makeBinaryOperation(code[0], code[1], TAC_LE);
        break;
    case AST_GE:
        result = makeBinaryOperation(code[0], code[1], TAC_GE);
        break;
    case AST_EQ:
        result = makeBinaryOperation(code[0], code[1], TAC_EQ);
        break;
    case AST_DIF:
        result = makeBinaryOperation(code[0], code[1], TAC_DIF);
        break;
    case AST_ASSIGNMENT:
        result = tacJoin(code[0], tacCreate(TAC_COPY, node->symbol, code[0] ? code[0]->res : 0, code[1] ? code[1]->res : 0));
        break;
    case AST_FUNC_DEC:
        result = makeFunc(node, code[1], code[2]);
        break;
    case AST_PRINT:
        result = makeBinaryOperation(code[0], code[1], TAC_PRINT);
        break;
    case AST_READ:
        result = tacCreate(TAC_READ, node->symbol, 0, 0);
        break;
    case AST_RETURN:
        result = tacJoin(code[0], tacCreate(TAC_RETURN, code[0]->res, 0, 0));
        break;
    case AST_FUNC_CALL:
        result = tacJoin(code[1], tacCreate(TAC_FUNC_CALL, makeTemp(), code[0] ? code[0]->res : 0, 0));
        break;
    case AST_IF:
        result = makeIfThenElse(code[0], code[1], code[2]);
        break;
    case AST_GLOBAL_VECTOR_DECLARATION:
        result = tacJoin(tacCreate(TAC_VEC_DECLARATION, node->symbol, 0, 0), code[1]);
        break;
    case AST_WHILE:
        result = makeWhile(code[0], code[1]);
        break;
    case AST_GLOBAL_VARIABLE_DECLARATION:
         result = tacJoin(tacCreate(TAC_VAR_DECLARATION, node->symbol, 0, 0), code[1]); 
        break;
    default:
        result = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3])));
    }

    return result;
}

TAC* makeFunc(AST* node, TAC* param, TAC* cFunc){
    HASH_NODE* labelFuncBegin = makeLabel();
    HASH_NODE* labelFuncEnd = makeLabel();

    TAC* funcBeg = tacCreate(TAC_BEGIN_FUN, node->symbol, labelFuncBegin, 0);
    TAC* funcEnd = tacCreate(TAC_END_FUN, node->symbol, labelFuncEnd, 0);
    return tacJoin(funcBeg, tacJoin(param, tacJoin(cFunc, funcEnd)));
}

TAC *makeWhile(TAC *expr, TAC *body)
{
    HASH_NODE *labelBefore = makeLabel();
    HASH_NODE *labelAfter = makeLabel();

    TAC *tacLabelBefore = tacCreate(TAC_LABEL, labelBefore, 0, 0);
    TAC *tacLabelAfter = tacCreate(TAC_LABEL, labelAfter, 0, 0);

    TAC *tacJumpFalse = makeCondJump(tacLabelAfter, expr);

    TAC *tacJumpBefore = makeIncondJump(tacLabelBefore);

    TAC *labelExprAndJump = tacJoin(tacLabelBefore, tacJoin(expr, tacJumpFalse));
    TAC *blockWithJumpAndLabel = tacJoin(tacJoin(body, tacJumpBefore), tacLabelAfter);

    TAC *fullWhile = tacJoin(labelExprAndJump, blockWithJumpAndLabel);

    return fullWhile;
}

TAC* makeCondJump(TAC* label, TAC* expr) {
    return tacCreate(TAC_JFALSE, label ? label->res : 0, expr ? expr->res : 0, 0);
}

TAC* makeIncondJump(TAC* label) {
    return tacCreate(TAC_JUMP, label ? label->res : 0, 0, 0);
}

TAC* makeIfThenElse(TAC* expr, TAC* cIf, TAC* cElse){
    HASH_NODE* newLabelElse = makeLabel();
    HASH_NODE* newLabelCont = makeLabel();

    TAC* ifThenElse = tacCreate(TAC_IF_ELSE, expr?expr->res:0, newLabelElse, 0);
    TAC* labelElse = tacCreate(TAC_LABEL, newLabelElse, 0, 0);
    TAC* labelCont = tacCreate(TAC_LABEL, newLabelCont, 0, 0);
    TAC* jump = tacCreate(TAC_JUMP, newLabelCont, 0, 0);
    return tacJoin(expr, tacJoin(ifThenElse, tacJoin(cIf, tacJoin(jump, tacJoin(labelElse, tacJoin(cElse, labelCont))))));
}
