target: cminus.o

cminus.tab.c: cminus.y
	bison cminus.y

cminus.o: cminus.tab.c globals.h token.h tree.o
	gcc -o cminus.o -c cminus.tab.c

tree.o: tree.c tree.h
	gcc -c tree.c
