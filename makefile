CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -g -O0
YFLAGS = --verbose --debug --defines
LFLAGS = --yylineno
all: yacc lex scanner

lex: lexer.l
	lex $(LFLAGS) lexer.l

scanner: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) $^ -ll -o $@

yacc: grammar.y
	yacc $(YFLAGS) grammar.y

clean: 
	rm -f lex.yy.c 
	rm -f scanner
	rm -f parser
	rm -f y.tab.c
	rm -f y.output
	rm -f y.tab.h

.PHONY: clean all
