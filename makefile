CC = g++
CFLAGS = -Wall -pedantic -g -O0
YFLAGS = --verbose --debug --defines
LFLAGS = --yylineno

all: lex.yy.c y.tab.c parser

lex.yy.c: lexer.l
	flex $(LFLAGS) lexer.l

scanner: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) $^ -ll -o $@

y.tab.c: grammar.y
	yacc $(YFLAGS) grammar.y
parser: lex.yy.c y.tab.c TreePrinter.o Node/*.cpp SemanticErrors.o TypeMap.o SymbolTable.o TreeWalker.o

	$(CC) $(CFLAGS) $^ -o $@ -lfl

TreeWalker.o: TreeWalker/SymbolTableGenerator.cpp
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

TreePrinter.o: TreePrinter/TreePrinter.cpp
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

SemanticErrors.o: Errors/SemanticErrors.cpp
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

TypeMap.o: Errors/TypeMap.cpp
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

SymbolTable.o: SymbolTable/SymbolTable.cpp
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

clean: 
	rm -f *.o
	rm -f lex.yy.c 
	rm -f scanner
	rm -f parser
	rm -f y.tab.c
	rm -f y.output
	rm -f y.tab.h

.PHONY: clean all
