CC = g++
CFLAGS = -Wall -pedantic -g -O0 
YFLAGS = --verbose --debug --defines
LFLAGS = --yylineno
objects := $(patsubst %.c,%.o,$(wildcard Node/*.cpp Errors/*.cpp TreePrinter/*.cpp SymbolTable/*.cpp TreeWalker/*.cpp))

all: yacc lex TreePrinter Node Errors SymbolTable TreeWalker parser 

lex: lexer.l
	flex $(LFLAGS) lexer.l

scanner: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) $^ -ll -o $@

yacc: grammar.y
	yacc $(YFLAGS) grammar.y

parser: $(objects)
	$(CC) $(CFLAGS) lex.yy.c y.tab.c $(objects) -o $@ -lfl

TreePrinter: TreePrinter/*.cpp
	$(CC) $(CFLAGS) -c $^  

Node: Node/*.cpp
	$(CC) $(CFLAGS) -c $^  

SymbolTable: SymbolTable/*.cpp
	$(CC) $(CFLAGS) -c $^  

Errors: Errors/*.cpp
	$(CC) $(CFLAGS) -c $^  

TreeWalker: TreeWalker/*.cpp
	$(CC) $(CFLAGS) -c $^  

clean: 
	rm -f lex.yy.c 
	rm -f scanner
	rm -f parser
	rm -f y.tab.c
	rm -f y.output
	rm -f y.tab.h
	rm -f *.o

.PHONY: clean all
