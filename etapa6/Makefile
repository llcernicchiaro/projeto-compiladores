# Douglas Gehring - 00243682
# Lorenzo Cernicchiaro - 00287718

etapa6: y.tab.o lex.yy.o ast.o hash.o semantic.o tac.o asm.o main.o 
	gcc y.tab.o lex.yy.o ast.o hash.o semantic.o tac.o asm.o main.o -o etapa6

main.o: main.c
	gcc -c main.c

asm.o: asm.c
	gcc -c asm.c

tac.o: tac.c
	gcc -c tac.c

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
	rm lex.yy.c y.tab.c y.tab.h y.output *.o etapa6
