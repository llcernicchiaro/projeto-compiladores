// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#include "semantic.h"

int semanticErrors = 0;

void checkAndSetDeclarations(AST *node)
{
    int i;

    if (node == 0)
    {
        return;
    }

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

        setDataTypeWith(node);

        break;
    }

    for(i=0; i< MAXSONS; ++i)
    {
        checkAndSetDeclarations(node->son[i]);
    }
}

void setDataTypeWith(AST *node) 
{
    if(node->son[0])
    {
        switch(node->son[0]->type)
        {
            case AST_INT:
                node->symbol->type = DATA_TYPE_INT;
                break;
            case AST_FLOAT:
                node->symbol->type = DATA_TYPE_FLOAT;
                break;
            case AST_CHAR:
                node->symbol->type = DATA_TYPE_CHAR;
                break;
        }
    }
}

void checkUndeclared()
{
    semanticErrors += hashCheckUndeclared();
}
