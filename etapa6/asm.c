#include "asm.h"

// 	.section	__DATA,__data
// _a: .long	2                               ## 0x2

// 	.section	__TEXT,__cstring,cstring_literals
// L_.str:                                 ## @.str
// 	.asciz	"%d"

void generateASM(TAC *tac, FILE* fout) {
    
    // Sempre no inicio estrutura padrao:

    printASMInitStatement(fout);

    // Varrer a lista de tacs

    bool isCurrentFunctionMain = false;

    for(;tac; tac=tac->next) 
    {
        switch(tac->type)
        {
            case TAC_SYMBOL:
            // Fazer nada
            break;
            case TAC_ADD:
                printASMElement(fout, "TAC_ADD");
            break;
            case TAC_SUB:
                printASMElement(fout, "TAC_SUB");
            break;
            case TAC_MULT:
                printASMElement(fout, "TAC_MULT");
            break;
            case TAC_DIV:
                printASMElement(fout, "TAC_DIV");
            break;
            case TAC_COPY:
                printASMElement(fout, "TAC_COPY");
            break;
            case TAC_LABEL:
                printASMElement(fout, "TAC_LABEL");
            break;
            case TAC_BEGIN_FUN:
                isCurrentFunctionMain = isMainFunction(tac->res->text);
                printASMBeginOfFunction(fout, tac->res->text);
            break;
            case TAC_END_FUN:
                printASMEndOfFunction(fout, isCurrentFunctionMain);
                isCurrentFunctionMain = false;
            break;
            case TAC_IFZ:
                printASMElement(fout, "TAC_IFZ");
            break;
            case TAC_JUMP:
                printASMElement(fout, "TAC_JUMP");
            break;
            case TAC_CALL:
                printASMElement(fout, "TAC_CALL");
            break;
            case TAC_ARG:
                printASMElement(fout, "TAC_ARG");
            break;
            case TAC_RETURN:
                printASMElement(fout, "TAC_RETURN");
            break;
            case TAC_PRINT:
                printASMElement(fout, "TAC_PRINT");
            break;
            case TAC_READ:
                printASMElement(fout, "TAC_READ");
            break;
            case TAC_LESS:
                printASMElement(fout, "TAC_LESS");
            break;
            case TAC_GREATER:
                printASMElement(fout, "TAC_GREATER");
            break;
            case TAC_OR:
                printASMElement(fout, "TAC_OR");
            break;
            case TAC_AND:
                printASMElement(fout, "TAC_AND");
            break;
            case TAC_LE:
                printASMElement(fout, "TAC_LE");
            break;
            case TAC_GE:
                printASMElement(fout, "TAC_GE");
            break;
            case TAC_EQ:
                printASMElement(fout, "TAC_EQ");
            break;
            case TAC_DIF:
                printASMElement(fout, "TAC_DIF");
            break;
            case TAC_FUNC_CALL:
                printASMElement(fout, "TAC_FUNC_CALL");
            break;
            case TAC_JFALSE:
                printASMElement(fout, "TAC_JFALSE");
            break;
            case TAC_IF_ELSE:
                printASMElement(fout, "TAC_IF_ELSE");
            break;
            case TAC_VEC_DECLARATION:
                printASMElement(fout, "TAC_VEC_DECLARATION");
            break;
            case TAC_VAR_DECLARATION:
                printASMElement(fout, "TAC_VAR_DECLARATION");
            break;
            case TAC_PARAM:
                printASMElement(fout, "TAC_PARAM");
            break;
            default:
                printf("TAC TYPE: %d", tac->type);
                break;
        }
    }

    hashPrintASM(fout);

    printASMEndStatement(fout);
    
}

bool isMainFunction(char *identifier) 
{
    return strcmp(identifier, "main") == 0;
}

void printASMElement(FILE* fout, char* element) 
{
    //fprintf(fout, "##%s\n", element);
}

void printASMEndStatement(FILE *fout) 
{
    fprintf(fout, ".subsections_via_symbols\n");
}

void printASMInitStatement(FILE* fout) 
{
    fprintf(fout, "\t.section	__TEXT,__text,regular,pure_instructions\n");
}

void printASMBeginOfFunction(FILE* fout, char* identifier) 
{
    fprintf(
        fout, 
        "\t.globl	_%s                           ## -- Begin function %s\n"
        "_%s:                                  ## @%s\n"
        "\t.cfi_startproc\n"
        "## %%bb.0:\n"
        "\tpushq	%%rbp\n"
        "\tmovq	%%rsp, %%rbp\n"
        "\t.cfi_def_cfa_register %%rbp\n",
        identifier, identifier, identifier, identifier
    );
}

void printASMEndOfFunction(FILE* fout, bool isMain) 
{
    if (isMain)
    {
        fprintf(fout, "\txorl	%%eax, %%eax\n\tpopq	%%rbp\n\tretq\n\t.cfi_endproc\n                           ## -- End of function\n");
    } 
    else 
    {
        fprintf(fout, "\tpopq	%%rbp\n\tretq\n\t.cfi_endproc\n                           ## -- End of function\n");
    }
}