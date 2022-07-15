// Douglas Gehring - 00243682
// Lorenzo Cernicchiaro - 00287718
%{
    #include <stdio.h>
    #include <stdlib.h>
    extern int yylineno;

    int yylex(void);
    void yyerror (char *s);
%}

%start program

%token KW_CHAR          
%token KW_INT            
%token KW_FLOAT          

%token KW_IF             
%token KW_ELSE           
%token KW_WHILE          
%token KW_READ          
%token KW_PRINT          
%token KW_RETURN         

%token ASSIGNMENT     
%token OPERATOR_LE       
%token OPERATOR_GE       
%token OPERATOR_EQ       
%token OPERATOR_DIF      

%token TK_IDENTIFIER     

%token LIT_INTEGER       
%token LIT_FLOAT
%token LIT_CHAR          
%token LIT_STRING        

%token TOKEN_ERROR    

/* %left '<' '>' OPERATOR_EQ OPERATOR_DIF OPERATOR_LE OPERATOR_GE */
%left '+' '-'
%left '*' '/'

%%

program: declarationList;

declarationList: declaration declarationList | ;

declaration: globalVariableDeclaration | globalVectorDeclaration | funDec;

type: KW_FLOAT | KW_CHAR | KW_INT;

literal: LIT_CHAR | LIT_FLOAT | LIT_INTEGER;

literalList: literal literalListTail;

literalListTail: ' ' literal | ;

initialization: '(' literal ')';

optionalInitialization: literalList | ;

globalVariableDeclaration: type TK_IDENTIFIER initialization ';';

globalVectorDeclaration: type TK_IDENTIFIER '[' LIT_INTEGER ']' optionalInitialization ';';


vecDec: KW_FLOAT TK_IDENTIFIER '[' LIT_INTEGER ']' arrayInit ';'
    | KW_CHAR TK_IDENTIFIER '[' LIT_INTEGER ']' arrayInit ';'
    | KW_INT TK_IDENTIFIER '[' LIT_INTEGER ']' arrayInit ';'
    ;

funDec: KW_FLOAT TK_IDENTIFIER '(' paramList ')' cmd
    | KW_CHAR TK_IDENTIFIER '(' paramList ')' cmd
    | KW_INT TK_IDENTIFIER '(' paramList ')' cmd
    | KW_FLOAT TK_IDENTIFIER '(' ')' cmd
    | KW_CHAR TK_IDENTIFIER '(' ')' cmd
    | KW_INT TK_IDENTIFIER '(' ')' cmd
    ;

charIntType: KW_CHAR
    | KW_INT
    ;

litCharInt: LIT_CHAR
    | LIT_INTEGER
    ;

arrayInit: ':' valorVetor
    | 
    ;

valorVetor: litCharInt
    | litCharInt valorVetor
    ;


cmdBlock: '{' cmdList '}'
    ;

cmdList: cmd ';' cmdList
    | label cmdList
    |  
    ;

assignment: TK_IDENTIFIER '=' expr
    | TK_IDENTIFIER '[' expr ']' '=' expr
    ;

expr: litCharInt
    | TK_IDENTIFIER
    | TK_IDENTIFIER '[' expr ']'
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '>' expr
    | expr '<' expr
    | expr OPERATOR_EQ expr
    | expr OPERATOR_DIF expr
    | expr OPERATOR_LE expr
    | expr OPERATOR_GE expr
    | '(' expr ')'
    | KW_READ
    | funcCall
    ;

printCmd: KW_PRINT printParams
    ;

printParams: printParam
    | printParam ',' printParams
    ;

printParam: LIT_STRING
    | expr
    ;

cmd: assignment 
    | printCmd 
    | whileCmd 
    | ifCmd 
    | returnCmd
    | cmdBlock
    |
    ;

whileCmd: KW_WHILE expr cmd
    ;

funcCall: TK_IDENTIFIER '(' paramsFuncCall ')'
    | TK_IDENTIFIER '(' ')'
    ;

paramsFuncCall: expr ',' paramsFuncCall
    | expr
    ;

label: TK_IDENTIFIER ':'

ifCmd: KW_IF expr cmd elseIfOptional
    ;

elseIfOptional: KW_ELSE cmd
    |
    ;

returnCmd: KW_RETURN expr
    ;

paramList: param ',' paramList
    | param
    ;

param: type TK_IDENTIFIER
    ;

%%

void yyerror(char *s)
{
    int lineNum = getLineNumber();
    fprintf(stdout, "%s na linha: %i\n", s, lineNum);
    
    exit(3);
}
