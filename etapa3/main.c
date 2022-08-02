// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#include <stdio.h>
#include "y.tab.c"

int main(int argc, char **argv)
{

    int tok;

    if (argc < 3)
    {

        fprintf(stderr, "Call ./etapa3 in_file_name out_file_name\n");
        exit(1);
    }

    yyin = fopen(argv[1], "r");

    if (yyin == 0)
    {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        exit(2);
    }

    if ((out = fopen(argv[2], "w+")) == 0)
    {
        printf("Cannot open the result file!\n");
        exit(1);
    }

    yyparse();

    printf("\nLineNumber %d", lineNumber);

    hashPrint();

    fprintf(stderr, "Legal! Compilation Successfull");

    exit(0);
}
