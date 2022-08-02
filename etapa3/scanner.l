%{
	// Douglas Gehring - 00243682
	// Lorenzo Cernicchiaro - 00287718

	#include "y.tab.h"
	#include "hash.h"
	int running = 1;
	int lineNumber = 1;
	int isRunning();
	int getLineNumber();
	void initMe();
%}

%x multilineComment
digit [0-9]
letter [_A-Za-z]

%%

[\t ] // Tabulação e espaço


"char" { return KW_CHAR; }
"int" { return KW_INT; }
"float" { return KW_FLOAT; } 

"if" { return KW_IF; }
"else" { return KW_ELSE; }
"while" { return KW_WHILE; }
"read" { return KW_READ; }
"print" { return KW_PRINT; }
"return" { return KW_RETURN; }

"<-" { return ASSIGNMENT; }
"<=" { return OPERATOR_LE; }
">=" { return OPERATOR_GE; }
"==" { return OPERATOR_EQ; }
"!=" { return OPERATOR_DIF; }


{letter}* { yylval.symbol = hashInsert(yytext, TK_IDENTIFIER); return TK_IDENTIFIER; }
{digit}+ { yylval.symbol = hashInsert(yytext, LIT_INTEGER); return LIT_INTEGER; }
\'[^\n']\' { yylval.symbol = hashInsert(yytext, LIT_CHAR); return LIT_CHAR; }
\"([^\\\"]|\\.)*\" { yylval.symbol = hashInsert(yytext, LIT_STRING); return LIT_STRING; }
{digit}+,{digit}+ { yylval.symbol = hashInsert(yytext, LIT_FLOAT); return LIT_FLOAT; }

"-" { return yytext[0]; }
";" { return yytext[0]; }
"(" { return yytext[0]; }
")" { return yytext[0]; }
"[" { return yytext[0]; }
"]" { return yytext[0]; }
"{" { return yytext[0]; }
"}" { return yytext[0]; }
"+" { return yytext[0]; }
"." { return yytext[0]; }
"/" { return yytext[0]; }
"<" { return yytext[0]; }
">" { return yytext[0]; }
"&" { return yytext[0]; }
"|" { return yytext[0]; }
"~" { return yytext[0]; }

"\n" { lineNumber++; }

"\\#" { BEGIN(multilineComment); }

<multilineComment>"#\\" { BEGIN(INITIAL); }
<multilineComment>. { }
<multilineComment>"\n" { lineNumber++; }


"##"[^\n]*  { } // Single line comment


. { return TOKEN_ERROR; }
%%

#include "main.c"
#include "hash.c"

int yywrap() {
	running = 0;
    return 1;
}

int isRunning() {
	return running;
}

int getLineNumber() {
	return lineNumber;
}

void initMe() {
	hashInit();
}