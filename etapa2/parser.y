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

%left '~' '&' '|'
%left '<' '>' OPERATOR_EQ OPERATOR_DIF OPERATOR_LE OPERATOR_GE
%left '+' '-'
%left '.' '/'

%%

program: declarationList;

declarationList: declaration declarationList | ;

declaration: globalVariableDeclaration | globalVectorDeclaration | functionDeclaration;

type: KW_FLOAT | KW_CHAR | KW_INT;

literal: LIT_CHAR | LIT_FLOAT | LIT_INTEGER;

literalList: literal literalListTail;

literalListTail: literal literalListTail | ;

initialization: '(' literal ')';

optionalInitialization: literalList | ;

globalVariableDeclaration: type TK_IDENTIFIER initialization ';';

globalVectorDeclaration: type TK_IDENTIFIER '[' LIT_INTEGER ']' optionalInitialization ';';

param: type TK_IDENTIFIER; 

optionalParamsList: param optionalParamsList | ;

functionDeclaration: type TK_IDENTIFIER '(' optionalParamsList ')' commandBlock;

commandBlock: '{' commandList '}';

commandList: command commandListTail;

commandListTail: ';' command commandListTail | ;

command: assignment 
    | readCommand
    | printCommand
    | returnCommand
    | ifCommand 
    | whileCommand 
    | commandBlock
    | ;

assignment: TK_IDENTIFIER ASSIGNMENT expression | 
    TK_IDENTIFIER '[' expression ']' ASSIGNMENT expression
    ;

readCommand: KW_READ TK_IDENTIFIER optionalIndex;

optionalIndex: '[' expression ']' | ;

printCommand: KW_PRINT printParamList;

printParam: LIT_STRING | expression;

printParamList: printParam printParamList | ;

returnCommand: KW_RETURN expression;

ifCommand: KW_IF '(' expression ')' command optionalElse;

optionalElse: KW_ELSE command | ;

whileCommand: KW_WHILE '(' expression ')' command;

expression: TK_IDENTIFIER
    | TK_IDENTIFIER '[' expression ']'
    | literal
    | expression '+' expression
    | expression '-' expression
    | expression '.' expression
    | expression '/' expression
    | expression '>' expression
    | expression '<' expression
    | expression OPERATOR_LE expression
    | expression OPERATOR_GE expression
    | expression OPERATOR_EQ expression
    | expression OPERATOR_DIF expression
    | expression '&' expression
    | expression '|' expression
    | expression '~' expression
    | '(' expression ')'
    | functionCall
    ;

functionCall: TK_IDENTIFIER '(' functionParamsList ')';

functionParamsList: expression functionParamsList | ;

%%

void yyerror(char *s)
{
    int lineNum = getLineNumber();
    fprintf(stdout, "%s na linha: %i\n", s, lineNum);
    
    exit(3);
}
