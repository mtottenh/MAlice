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
parser: lex.yy.c y.tab.c TreePrinter.o SemanticErrors.o TypeMap.o SymbolTable.o TreeWalker.o NArrayAccess.o NAssignment.o NBinOp.o NCharLit.o NCodeBlock.o NConditional.o NDec.o NDeclarationBlock.o NEndIf.o NFunctionDeclaration.o NIdentifier.o NInc.o NInput.o NInteger.o NLoop.o NMethodCall.o Node.o NParamBlock.o NParamDeclarationBlock.o NPredicate.o NPrint.o NReturn.o NStatementList.o NString.o NStringLit.o NUnaryOp.o NVariableDeclaration.o

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

NArrayAccess.o: Node/NArrayAccess.cpp
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl
NAssignment.o: Node/NAssignment.cpp
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl
NBinOp.o: Node/NBinOp.cpp
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl
NCharLit.o: Node/NCharLit.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NCodeBlock.o: Node/NCodeBlock.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NConditional.o: Node/NConditional.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NDec.o: Node/NDec.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NDeclarationBlock.o: Node/NDeclarationBlock.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NEndIf.o: Node/NEndIf.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NFunctionDeclaration.o: Node/NFunctionDeclaration.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NIdentifier.o: Node/NIdentifier.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NInc.o: Node/NInc.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NInput.o: Node/NInput.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NInteger.o: Node/NInteger.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NLoop.o: Node/NLoop.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NMethodCall.o: Node/NMethodCall.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

Node.o: Node/Node.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NParamBlock.o: Node/NParamBlock.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NParamDeclarationBlock.o: Node/NParamDeclarationBlock.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NPredicate.o: Node/NPredicate.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NPrint.o: Node/NPrint.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NReturn.o: Node/NReturn.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NStatementList.o: Node/NStatementList.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NString.o: Node/NString.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NStringLit.o: Node/NStringLit.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NUnaryOp.o: Node/NUnaryOp.cpp 
	$(CC) $(CFLAGS) $^ -c -o $@ -lfl

NVariableDeclaration.o: Node/NVariableDeclaration.cpp 
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
