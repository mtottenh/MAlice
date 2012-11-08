all: lex spec

lex: spec.l 
	lex spec.l

spec: lex.yy.c
	gcc $^ -ll -o $@

clean: 
	rm lex.yy.c spec

.PHONY: clean all
