// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ast.h"

AST *astCreate(int type, HASH_NODE *symbol, AST *son0, AST *son1, AST *son2, AST *son3)
{
    AST *new_node;
    new_node = (AST *)calloc(1, sizeof(AST));
    new_node->type = type;
    new_node->symbol = symbol;
    new_node->son[0] = son0;
    new_node->son[1] = son1;
    new_node->son[2] = son2;
    new_node->son[3] = son3;
    return new_node;
}

void astPrint(AST *node, int level)
{
    int i;
    if (node == 0)
    {
        return;
    }

    for (i = 0; i <= level; i++)
    {
        fprintf(stderr, "  ");
    }

    fprintf(stderr, "AST( ");
    switch (node->type)
    {
    case AST_SYMBOL:
        fprintf(stderr, "AST_SYMBOL");
        break;
    case AST_ADD:
        fprintf(stderr, "AST_ADD");
        break;
    case AST_SUB:
        fprintf(stderr, "AST_SUB");
        break;
    case AST_MUL:
        fprintf(stderr, "AST_MUL");
        break;
    case AST_DIV:
        fprintf(stderr, "AST_DIV");
        break;
    case AST_EQ:
        fprintf(stderr, "AST_EQ");
        break;
    case AST_GE:
        fprintf(stderr, "AST_GE");
        break;
    case AST_LE:
        fprintf(stderr, "AST_LE");
        break;
    case AST_GREATER_THAN:
        fprintf(stderr, "AST_GT");
        break;
    case AST_LESS_THAN:
        fprintf(stderr, "AST_LT");
        break;
    case AST_ASSIGNMENT:
        fprintf(stderr, "AST_ASSIGNMENT");
        break;
    case AST_LITERAL_LIST:
        fprintf(stderr, "AST_LITERAL_LIST");
        break;
    case AST_LITERAL_LIST_TAIL:
        fprintf(stderr, "AST_LITERAL_LIST_TAIL");
        break;
    case AST_INITIALIZATION:
        fprintf(stderr, "AST_INITIALIZATION");
        break;
    case AST_OPTIONAL_INITIALIZATION:
        fprintf(stderr, "AST_OPTIONAL_INITIALIZATION");
        break;
    case AST_CMD_LIST:
        fprintf(stderr, "AST_CMD_LIST");
        break;
    case AST_CMD_LIST_TAIL:
        fprintf(stderr, "AST_CMD_LIST_TAIL");
        break;
    case AST_OPTIONAL_INDEX:
        fprintf(stderr, "AST_OPTIONAL_INDEX");
        break;
    case AST_AND:
        fprintf(stderr, "AST_AND");
        break;
    case AST_OR:
        fprintf(stderr, "AST_OR");
        break;
    case AST_NOT:
        fprintf(stderr, "AST_NOT");
        break;
    case AST_CHAR:
        fprintf(stderr, "AST_CHAR");
        break;
    case AST_INT:
        fprintf(stderr, "AST_INT");
        break;
    case AST_READ:
        fprintf(stderr, "AST_READ");
        break;
    case AST_WHILE:
        fprintf(stderr, "AST_WHILE");
        break;
    case AST_FUNC_CALL:
        fprintf(stderr, "AST_FUNC_CALL");
        break;
    case AST_PARAMS_FUNC_CALL:
        fprintf(stderr, "AST_PARAMS_FUNC_CALL");
        break;
    case AST_IF:
        fprintf(stderr, "AST_IF");
        break;
    case AST_ELSE:
        fprintf(stderr, "AST_ELSE");
        break;
    case AST_PRINT_PARAM_LIST:
        fprintf(stderr, "AST_PRINT_PARAM_LIST");
        break;
    case AST_RETURN:
        fprintf(stderr, "AST_RETURN");
        break;
    case AST_OPTIONAL_PARAMS_LIST:
        fprintf(stderr, "AST_OPTIONAL_PARAMS_LIST");
        break;
    case AST_PARAM:
        fprintf(stderr, "AST_PARAM");
        break;
    case AST_PRINT:
        fprintf(stderr, "AST_PRINT");
        break;
    case AST_FLOAT:
        fprintf(stderr, "AST_FLOAT");
        break;
    case AST_FUNC_DEC:
        fprintf(stderr, "AST_FUNC_DEC");
        break;
    case AST_GLOBAL_VECTOR_DECLARATION:
        fprintf(stderr, "AST_GLOBAL_VECTOR_DECLARATION");
        break;
    case AST_GLOBAL_VARIABLE_DECLARATION:
        fprintf(stderr, "AST_GLOBAL_VARIABLE_DECLARATION");
        break;
    case AST_DEC_LIST:
        fprintf(stderr, "AST_DEC_LIST");
        break;
    case AST_CMD_BLOCK:
        fprintf(stderr, "AST_CMD_BLOCK");
        break;
    case AST_ARRAY_ACCESS:
        fprintf(stderr, "AST_ARRAY_ACCESS");
        break;
    case AST_DIF:
        fprintf(stderr, "AST_DIF");
        break;
    case AST_BRACKETS_EXPR:
        fprintf(stderr, "AST_BRACKETS_EXPR");
        break;
    default:
        fprintf(stderr, "AST_UNKNOWN) %d \n", node->type);
        break;
    }

    level++;

    if (node->symbol)
        fprintf(stderr, ", %s)\n", node->symbol->text);
    else
        fprintf(stderr, ", 0)\n");

    for (i = 0; i < MAXSONS; i++)
        astPrint(node->son[i], level);
}

void generateSource(AST *node, FILE *out)
{

    AST *temp = node;

    if (temp == 0)
        return;

    switch (temp->type)
    {
    case AST_ADD:
        generateSource(temp->son[0], out);
        fprintf(out, " + ");
        generateSource(temp->son[1], out);
        break;
    case AST_SUB:
        generateSource(temp->son[0], out);
        fprintf(out, " - ");
        generateSource(temp->son[1], out);
        break;
    case AST_MUL:
        generateSource(temp->son[0], out);
        fprintf(out, " . ");
        generateSource(temp->son[1], out);
        break;
    case AST_DIV:
        generateSource(temp->son[0], out);
        fprintf(out, " / ");
        generateSource(temp->son[1], out);
        break;
    case AST_DIF:
        generateSource(temp->son[0], out);
        fprintf(out, " != ");
        generateSource(temp->son[1], out);
        break;
    case AST_EQ:
        generateSource(temp->son[0], out);
        fprintf(out, " == ");
        generateSource(temp->son[1], out);
        break;
    case AST_GE:
        generateSource(temp->son[0], out);
        fprintf(out, ">= ");
        generateSource(temp->son[1], out);
        break;
    case AST_LE:
        generateSource(temp->son[0], out);
        fprintf(out, " <= ");
        generateSource(temp->son[1], out);
        break;
    case AST_GREATER_THAN:
        generateSource(temp->son[0], out);
        fprintf(out, " > ");
        generateSource(temp->son[1], out);
        break;
    case AST_LESS_THAN:
        generateSource(temp->son[0], out);
        fprintf(out, " < ");
        generateSource(temp->son[1], out);
        break;
    case AST_ARRAY_ACCESS:
        fprintf(out, "%s[", temp->symbol->text);
        generateSource(temp->son[0], out);
        fprintf(out, "]");
        break;
    case AST_ASSIGNMENT:
        fprintf(out, "%s ", temp->symbol->text);
        if (temp->son[1] != NULL)
        {
            fprintf(out, "[");
            generateSource(temp->son[0], out);
            fprintf(out, "] <- ");
            generateSource(temp->son[1], out);
        }
        else
        {
            fprintf(out, "<- ");
            generateSource(temp->son[0], out);
        }
        break;
    case AST_DEC_LIST:
        generateSource(temp->son[0], out);
        generateSource(temp->son[1], out);
        break;
    case AST_LITERAL_LIST:
        fprintf(out, " ");
        generateSource(temp->son[0], out);
        generateSource(temp->son[1], out);
        break;
    case AST_LITERAL_LIST_TAIL:
        fprintf(out, " ");
        generateSource(temp->son[0], out);
        generateSource(temp->son[1], out);
        break;
    case AST_INITIALIZATION:
        fprintf(out, "(");
        generateSource(temp->son[0], out);
        fprintf(out, ")");
        break;
    case AST_OPTIONAL_INITIALIZATION:
        generateSource(temp->son[0], out);
        break;
    case AST_GLOBAL_VARIABLE_DECLARATION:
        generateSource(temp->son[0], out);
        fprintf(out, "%s ", temp->symbol->text);
        generateSource(temp->son[1], out);
        fprintf(out, ";\n");
        break;
    case AST_GLOBAL_VECTOR_DECLARATION:
        generateSource(temp->son[0], out);
        fprintf(out, "%s[", temp->symbol->text);
        generateSource(temp->son[1], out);
        fprintf(out, "]");
        generateSource(temp->son[2], out);
        fprintf(out, ";\n");
        break;
    case AST_INT:
        fprintf(out, "int ");
        break;
    case AST_CHAR:
        fprintf(out, "char ");
        break;
    case AST_FLOAT:
        fprintf(out, "float ");
        break;
    case AST_CMD_BLOCK:
        fprintf(out, "\n{\n");
        generateSource(temp->son[0], out);
        fprintf(out, "\n}\n");
        break;
    case AST_CMD_LIST:
        generateSource(temp->son[0], out);
        generateSource(temp->son[1], out);
        break;
    case AST_CMD_LIST_TAIL:
        fprintf(out, ";\n");
        generateSource(temp->son[0], out);
        generateSource(temp->son[1], out);
        break;
    case AST_SYMBOL:
        fprintf(out, "%s", node->symbol->text);
        break;
    case AST_PRINT:
        fprintf(out, "print ");
        generateSource(temp->son[0], out);
        break;
    case AST_PRINT_PARAM_LIST:
        generateSource(temp->son[0], out);
        generateSource(temp->son[1], out);
        break;
    case AST_READ:
        fprintf(out, "read ");
        fprintf(out, "%s", node->symbol->text);
        generateSource(temp->son[0], out);
        break;
    case AST_OPTIONAL_INDEX:
        fprintf(out, "[");
        generateSource(temp->son[0], out);
        fprintf(out, "]");
        break;
    case AST_WHILE:
        fprintf(out, "while(");
        generateSource(temp->son[0], out);
        fprintf(out, ")");
        generateSource(temp->son[1], out);
        break;
    case AST_FUNC_CALL:
        fprintf(out, "%s(", node->symbol->text);
        generateSource(temp->son[0], out);
        fprintf(out, ")");
        break;
    case AST_PARAMS_FUNC_CALL:
        generateSource(temp->son[0], out);
        if (temp->son[1] != NULL)
        {
            fprintf(out, " ");
            generateSource(temp->son[1], out);
        }
        break;
    case AST_IF:
        fprintf(out, "if(");
        generateSource(temp->son[0], out);
        fprintf(out, ")");
        generateSource(temp->son[1], out);
        generateSource(temp->son[2], out);
        break;
    case AST_ELSE:
        fprintf(out, "else ");
        generateSource(temp->son[0], out);
        break;
    case AST_FUNC_DEC:
        generateSource(temp->son[0], out);
        fprintf(out, "%s(", temp->symbol->text);
        if (temp->son[2] != NULL)
        {
            generateSource(temp->son[1], out);
            fprintf(out, ") ");
            generateSource(temp->son[2], out);
        }
        else
        {
            fprintf(out, ") ");
            generateSource(temp->son[1], out);
        }
        break;
    case AST_PARAM:
        generateSource(temp->son[0], out);
        fprintf(out, "%s", temp->symbol->text);
        break;
    case AST_OPTIONAL_PARAMS_LIST:
        generateSource(temp->son[0], out);
        if (temp->son[1] != NULL)
        {
            fprintf(out, " ");
            generateSource(temp->son[1], out);
        }
        break;
    case AST_RETURN:
        fprintf(out, "return ");
        generateSource(temp->son[0], out);
        break;
    case AST_NOT:
        fprintf(out, "~ ");
        generateSource(temp->son[0], out);
        break;
    case AST_OR:
        generateSource(temp->son[0], out);
        fprintf(out, " | ");
        generateSource(temp->son[1], out);
        break;
    case AST_AND:
        generateSource(temp->son[0], out);
        fprintf(out, " & ");
        generateSource(temp->son[1], out);
        break;
    case AST_BRACKETS_EXPR:
        fprintf(out, "(");
        generateSource(temp->son[0], out);
        fprintf(out, ")");
        break;
    default:
        fprintf(stderr, "UNKNOWN(%s)\n", temp->symbol->text);
        break;
    }

    fflush(out);
}

void closeFile(FILE *code)
{
    fflush(code);
    fclose(code);
}
