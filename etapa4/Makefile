etapa4: y.tab.o lex.yy.o ast.o hash.o semantic.o main.o 
	gcc y.tab.o lex.yy.o ast.o hash.o semantic.o main.o -o etapa4

main.o: main.c
	gcc -c main.c

semantic.o: semantic.c
	gcc -c semantic.c

hash.o: hash.c
	gcc -c hash.c

ast.o: ast.c
	gcc -c ast.c

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

y.tab.o: y.tab.c	
	gcc -c y.tab.c

y.tab.c: parser.y
	yacc -v -d parser.y

lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm lex.yy.c y.tab.c y.tab.h y.output *.o etapa4
