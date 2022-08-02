etapa3: lex.yy.c y.tab.c
	gcc lex.yy.c ast.o -o etapa3

ast.o: ast.c
		gcc -c ast.c

y.tab.c: parser.y
	yacc -v -d parser.y

lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm etapa3 lex.yy.c y.tab.c y.output *.o
