target = testree

$(target): testree.c tree.c
	gcc -o $@ testree.c tree.c
