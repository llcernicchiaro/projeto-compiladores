etapa1: lex.yy.c y.tab.c
	gcc -o etapa2 lex.yy.c 

y.tab.c: parser.y
	yacc -v -d parser.y

lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm etapa2 lex.yy.c y.tab.c
