lex_source=lex.yy.c symtab.c tree.c

exec: cm.l globals.h symtab.h tree.h token.h
	flex cm.l
	gcc -o lex $(lex_source)

test: exec test.c
	cat test.c | ./lex

cm.tab.h: cm.y
	bison -d cm.y
	

clean:
	rm -f lex lex.yy.c *.o
