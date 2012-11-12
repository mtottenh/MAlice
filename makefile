all: yacc lex scanner

lex: spec.l
	lex --yylineno spec.l

scanner: lex.yy.c y.tab.h
	gcc $^ -ll -o $@

yacc: spec.y
	yacc --defines spec.y

clean: 
	rm -f lex.yy.c 
	rm -f scanner
	rm -f parser
	rm -f y.tab.c
	rm -f y.output
	rm -f y.tab.h

.PHONY: clean all
