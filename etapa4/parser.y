%{
  // Douglas Gehring - 00243682
  // Lorenzo Cernicchiaro - 00287718
    #include <stdio.h>
    #include <stdlib.h>
    #include "ast.h"
    #include "semantic.h"

    extern int yylineno;
    FILE *out = NULL;

    int yylex();
    void yyerror (char *s);
    int getLineNumber();
%}

%union
{
    HASH_NODE *symbol;
    AST *ast;
}

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

%token<symbol> TK_IDENTIFIER

%token<symbol> LIT_INTEGER
%token<symbol> LIT_FLOAT
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING

%token TOKEN_ERROR

%type<ast> program declarationList declaration type literal literalList literalListTail
%type<ast> initialization optionalInitialization globalVariableDeclaration param optionalParamsList
%type<ast> functionDeclaration commandBlock commandList commandListTail command assignment
%type<ast> readCommand optionalIndex printCommand printParam printParamList returnCommand
%type<ast> ifCommand optionalElse whileCommand expression functionCall functionParamsList globalVectorDeclaration

%left '~'
%left '&' '|'
%left '<' '>' OPERATOR_EQ OPERATOR_DIF OPERATOR_LE OPERATOR_GE
%left '+' '-'
%left '.' '/'

%%

program: declarationList  { $$ = $1; 
                            astPrint($1,0); 
                            generateSource($1, out);
                            checkAndSetDeclarations($1); 
                            checkUndeclared($1);
                            checkOperands($1);
                            checkParameters($1);
                            checkGlobalVectorDeclaration($1);
                            checkFunctionsReturnType($1);
                          }
  ;

declarationList: declaration declarationList { $$ = astCreate(AST_DEC_LIST, 0, $1, $2, 0, 0); }
  |                                          { $$ = 0; }
  ;

declaration: globalVariableDeclaration    { $$ = $1; }
  | globalVectorDeclaration               { $$ = $1; }
  | functionDeclaration                   { $$ = $1; }
  ;

type: KW_FLOAT    { $$ = astCreate(AST_FLOAT, 0, 0, 0, 0, 0); }
    | KW_CHAR     { $$ = astCreate(AST_CHAR, 0, 0, 0, 0, 0); }
    | KW_INT      { $$ = astCreate(AST_INT, 0, 0, 0, 0, 0); }
    ;

literal: LIT_CHAR { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
  | LIT_FLOAT     { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
  | LIT_INTEGER   { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
  ;

literalList: literal literalListTail { $$ = astCreate(AST_LITERAL_LIST, 0, $1, $2, 0, 0); }
  ;

literalListTail: literal literalListTail  { $$ = astCreate(AST_LITERAL_LIST_TAIL, 0, $1, $2, 0, 0); }
  |                                       { $$ = 0; }
  ;

initialization: '(' literal ')' { $$ = astCreate(AST_INITIALIZATION, 0, $2, 0, 0, 0); }
  ;

optionalInitialization: literalList  { $$ = astCreate(AST_OPTIONAL_INITIALIZATION, 0, $1, 0, 0, 0); }
  |                                  { $$ = 0; }
  ;

globalVariableDeclaration: type TK_IDENTIFIER initialization ';' { $$ = astCreate(AST_GLOBAL_VARIABLE_DECLARATION, $2, $1, $3, 0, 0); }
  ;

globalVectorDeclaration: type TK_IDENTIFIER '[' LIT_INTEGER ']' optionalInitialization ';' { $$ = astCreate(AST_GLOBAL_VECTOR_DECLARATION, $2, $1, astCreate(AST_SYMBOL, $4, 0, 0, 0, 0), $6, 0); }
  ;

param: type TK_IDENTIFIER { $$ = astCreate(AST_PARAM, $2, $1, 0, 0, 0); }
  ;

optionalParamsList: param optionalParamsList  { $$ = astCreate(AST_OPTIONAL_PARAMS_LIST, 0, $1, $2, 0, 0); }
  |                                           { $$ = 0; }
  ;

functionDeclaration: type TK_IDENTIFIER '(' optionalParamsList ')' commandBlock { $$ = astCreate(AST_FUNC_DEC, $2, $1, $4, $6, 0); }
  ;

commandBlock: '{' commandList '}' { $$ = astCreate(AST_CMD_BLOCK, 0, $2, 0, 0, 0); }
  ;

commandList: command commandListTail  { $$ = astCreate(AST_CMD_LIST, 0, $1, $2, 0, 0); }
  ;

commandListTail: ';' command commandListTail { $$ = astCreate(AST_CMD_LIST_TAIL, 0, $2, $3, 0, 0); }
  |                                          { $$ = 0; }
  ;

command: assignment   { $$ = $1; }
  | readCommand       { $$ = $1; }
  | printCommand      { $$ = $1; }
  | returnCommand     { $$ = $1; }
  | ifCommand         { $$ = $1; }
  | whileCommand      { $$ = $1; }
  | commandBlock      { $$ = $1; }
  |                   { $$ = 0; }
  ;

assignment: TK_IDENTIFIER ASSIGNMENT expression                 { $$ = astCreate(AST_ASSIGNMENT, $1, $3, 0, 0, 0); }
    | TK_IDENTIFIER '[' expression ']' ASSIGNMENT expression    { $$ = astCreate(AST_ASSIGNMENT, $1, $3, $6, 0, 0); }
    ;

readCommand: KW_READ TK_IDENTIFIER optionalIndex { $$ = astCreate(AST_READ, $2, $3, 0, 0, 0); }
    ;

optionalIndex: '[' expression ']' { $$ = astCreate(AST_OPTIONAL_INDEX, 0, $2, 0, 0, 0); }
    |                             { $$ = 0; }
    ;

printCommand: KW_PRINT printParamList { $$ = astCreate(AST_PRINT, 0, $2, 0, 0, 0); }
    ;

printParam: LIT_STRING { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    | expression         { $$ = $1; }
    ;

printParamList: printParam printParamList    { $$ = astCreate(AST_PRINT_PARAM_LIST, 0, $1, $2, 0, 0); }
    |                                        { $$ = 0; }
    ;

returnCommand: KW_RETURN expression { $$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0); }
    ;

ifCommand: KW_IF '(' expression ')' command optionalElse    { $$ = astCreate(AST_IF, 0, $3, $5, $6, 0); }
    ;

optionalElse: KW_ELSE command     { $$ = astCreate(AST_ELSE, 0, $2, 0, 0, 0); }
    |                             { $$ = 0; }
    ;

whileCommand: KW_WHILE '(' expression ')' command  { $$ = astCreate(AST_WHILE, 0, $3, $5, 0, 0); }
    ;

expression: TK_IDENTIFIER                   { $$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0); }
    | TK_IDENTIFIER '[' expression ']'      { $$ = astCreate(AST_ARRAY_ACCESS, $1, $3, 0, 0, 0); }
    | literal                               { $$ = $1; }
    | expression '+' expression             { $$ = astCreate(AST_ADD, 0, $1, $3, 0, 0); }
    | expression '-' expression             { $$ = astCreate(AST_SUB, 0, $1, $3, 0, 0); }
    | expression '.' expression             { $$ = astCreate(AST_MUL, 0, $1, $3, 0, 0); }
    | expression '/' expression             { $$ = astCreate(AST_DIV, 0, $1, $3, 0, 0); }
    | expression '>' expression             { $$ = astCreate(AST_GREATER_THAN, 0, $1, $3, 0, 0); }
    | expression '<' expression             { $$ = astCreate(AST_LESS_THAN, 0, $1, $3, 0, 0); }
    | expression OPERATOR_LE expression     { $$ = astCreate(AST_LE, 0, $1, $3, 0, 0); }
    | expression OPERATOR_GE expression     { $$ = astCreate(AST_GE, 0, $1, $3, 0, 0); }
    | expression OPERATOR_EQ expression     { $$ = astCreate(AST_EQ, 0, $1, $3, 0, 0); }
    | expression OPERATOR_DIF expression    { $$ = astCreate(AST_DIF, 0, $1, $3, 0, 0); }
    | expression '&' expression             { $$ = astCreate(AST_AND, 0, $1, $3, 0, 0); }
    | expression '|' expression             { $$ = astCreate(AST_OR, 0, $1, $3, 0, 0);  }
    | '~' expression                        { $$ = astCreate(AST_NOT, 0, $2, 0, 0, 0); }
    | '(' expression ')'                    { $$ = astCreate(AST_BRACKETS_EXPR, 0, $2, 0, 0, 0); }
    | functionCall                          { $$ = $1; }
    ;

functionCall: TK_IDENTIFIER '(' functionParamsList ')'  { $$ = astCreate(AST_FUNC_CALL, $1, $3, 0, 0, 0); }
    ;

functionParamsList: expression functionParamsList   { $$ = astCreate(AST_PARAMS_FUNC_CALL, 0, $1, $2, 0, 0); }
    |                                               { $$ = 0; }
    ;

%%

void yyerror(char *s)
{
    int lineNum = getLineNumber();
    fprintf(stdout, "%s na linha: %i\n", s, lineNum);

    exit(3);
}
