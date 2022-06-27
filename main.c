// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718

#include <stdio.h>

int main(int argc, char **argv)
{

    int tok;

    if (argc < 2)
    {

        fprintf(stderr, "Call ./etapa1 file_name\n");
        exit(1);
    }

    yyin = fopen(argv[1], "r");

    while (1)
    {
        tok = yylex();
        if (running == 0)
            break;
        printf("%d\n", tok);
    }

    printf("\nLineNumber %d\n", lineNumber);
    hashPrint();
}
