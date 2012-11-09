all: lex spec

lex: spec.l 
	lex --yylineno spec.l

spec: lex.yy.c
	gcc $^ -ll -o $@

clean: 
	rm -f lex.yy.c 
	rm -f spec
	rm -f y.tab.c
	rm -f y.output

.PHONY: clean all
