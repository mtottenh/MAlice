all: lex lexcomp

lex: spec.l 
	lex --yylineno spec.l

lexcomp: lex.yy.c
	gcc $^ -ll -o $@

clean: rm -rf lex.yy.c spec

.PHONY: clean all
