CC = g++
CFLAGS = -Wall -pedantic -g -O0
YFLAGS = --verbose --debug --defines
LFLAGS = --yylineno

scanner_test: yacc lex scanner
parser_test: yacc lex parser

lex: lexer.l
	flex $(LFLAGS) lexer.l

scanner: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) $^ -ll -o $@

yacc: grammar.y
	yacc $(YFLAGS) grammar.y
parser: lex.yy.c y.tab.c TreePrinter.cpp
	$(CC) $(CFLAGS) $^ -o $@ -lfl
clean: 
	rm -f lex.yy.c 
	rm -f scanner
	rm -f parser
	rm -f y.tab.c
	rm -f y.output
	rm -f y.tab.h

.PHONY: clean all
