etapa3: lex.yy.c y.tab.c
	gcc -o etapa3 lex.yy.c 

y.tab.c: parser.y
	yacc -v -d parser.y

lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm etapa3 lex.yy.c y.tab.c
