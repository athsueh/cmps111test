all: lex.yy.o shell

lex.yy.o: lex.yy.c
	cc -c lex.yy.c

lex.yy.c: lex.l
	flex lex.l
 
shell: myshell.o
	cc -o myshell myshell.o lex.yy.o -lfl
	
myshell.o: myshell.c
	cc -c myshell.c
	
clean:
	rm myshell.o lex.yy.c lex.yy.o
