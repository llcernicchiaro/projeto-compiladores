// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#include "asm.h"

char *getPercentChar(int type)
{
    switch (type)
    {
    case DATA_TYPE_INT:
        return ".percentInt";
    case DATA_TYPE_CHAR:
        return ".percentChar";
    case DATA_TYPE_FLOAT:
        return ".percentFloat";
    }
}

void generateASM(TAC *tac, FILE *fout)
{
    fprintf(fout,
            "## FILE_INIT\n"
            "\t.file	\"output.txt\"\n"
            "\t.text\n");

    fprintf(fout,
            "\n.percentInt:\n"
            "\t.string	\"%%d\\n\"");
    fprintf(fout,
            "\n.percentChar:\n"
            "\t.string	\"%%c\\n\"");
    fprintf(fout,
            "\n.percentFloat:\n"
            "\t.string	\"%%f\\n\"");

    for (; tac; tac = tac->next)
    {
        switch (tac->type)
        {
        case TAC_BEGIN_FUN:
            fprintf(fout,
                    "## TAC_BEGIN_FUN\n"
                    "\t.globl %s\n"
                    "%s:\n"
                    "\tpushq	%%rbp\n"
                    "\tmovq	%%rsp, %%rbp\n",
                    tac->res->text, tac->res->text);
            break;
        case TAC_END_FUN:
            fprintf(fout,
                    "## TAC_END_FUN\n"
                    "\tmovl	$0, %%eax\n"
                    "\tpopq	%%rbp\n"
                    "\tret\n");
            break;
        case TAC_PRINT:
            fprintf(fout, "## TAC_PRINT\n");
            if (tac->op1->type == SYMBOL_VARIABLE)
            {
                fprintf(fout,
                        "\tmovl	_%s(%%rip), %%eax\n"
                        "\tmovl	%%eax, %%esi\n",
                        removeChars(tac->op1->text, '"'));

                fprintf(fout,
                        "\tleaq	%s(%%rip), %%rdi\n"
                        "\tmovl	$0, %%eax\n"
                        "\tcall	printf@PLT\n",
                        getPercentChar(tac->op1->dataType));
            }
            else
            {
                fprintf(fout,
                        "\tleaq	_%s(%%rip), %%rdi\n"
                        "\tmovl	$0, %%eax\n"
                        "\tcall	printf@PLT\n",
                        removeChars(tac->op1->text, '"'));
            }

            break;
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

    fprintf(fout,
            "## FILE_END\n"
            "0:\n"
            "\t.string\t\"GNU\"\n"
            "1:\n"
            "\t.align 8\n"
            "\t.long\t0xc0000002\n"
            "\t.long\t3f - 2f\n"
            "2:\n"
            "\t.long	 0x3\n"
            "3:\n"
            "\t.align 8\n"
            "4:\n");
}

void printASMElement(FILE *fout, char *element)
{
    fprintf(fout, "## %s\n", element);
}
