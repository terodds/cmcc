CC = gcc
CFLAGS = -c -g -Wall -O3
EXEC = main

.SUFFIXES: .c .o

LEXCODE = cm.l
LEXSRC = cm.lex.c

BISONCODE = cm.y
BISONSRC = cm.tab.c
BISONHDR = cm.tab.h
BISONVERBOSE = cm.output

SRCS = main.c util.c symtab.c analyze.c $(LEXSRC) $(BISONSRC)
OBJS = $(SRCS:.c=.o)

$(EXEC): $(OBJS)
	$(CC) -o $@ -g $(OBJS)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS)

$(BISONSRC): $(LEXSRC) $(BISONCODE)
	bison -o $(BISONSRC) -vd $(BISONCODE)

$(LEXSRC): $(LEXCODE)
	flex -o $(LEXSRC) $(LEXCODE)

clean:
	rm -f $(OBJS) $(EXEC) $(LEXSRC) $(BISONSRC) $(BISONHDR) $(BISONVERBOSE)


test: $(EXEC)
	./test.sh $$CMINUS_HOME/testcase1/test$(file)e.txt
