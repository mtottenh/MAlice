CC = g++
CFLAGS = -Wall -pedantic -g -O0
YFLAGS = --verbose --debug --defines
LFLAGS = --yylineno
ODIR = obj

#A list of all objects our compiler needs to link into the executable
OBJS = NArrayAccess.o NAssignment.o NBinOp.o NCharLit.o NCodeBlock.o NConditional.o NDec.o NDeclarationBlock.o NEndIf.o NFunctionDeclaration.o NIdentifier.o NInc.o NInput.o NInteger.o NLoop.o NMethodCall.o Node.o NParamBlock.o NParamDeclarationBlock.o NPredicate.o NPrint.o NReturn.o NStatementList.o NStringLit.o NUnaryOp.o NVariableDeclaration.o TypeMap.o SemanticErrors.o TreePrinter.o SymbolTableGenerator.o SymbolTable.o FileLocation.o
# Messy but cant find a nicer way at present

OBJ=$(patsubst %,$(ODIR)/%,$(OBJS))

all: lex.yy.c y.tab.c compile

lex.yy.c: lexer.l
	flex $(LFLAGS) lexer.l

scanner: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) $^ -ll -o $@

y.tab.c: grammar.y
	yacc $(YFLAGS) grammar.y

compile: lex.yy.c y.tab.c $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -lfl


#Each Subdirectory makes all its files and places them in obj
$(ODIR)/%.o: TreeUtils/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $< -lfl

$(ODIR)/%.o: Errors/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $< -lfl

$(ODIR)/%.o: SymbolTable/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $< -lfl

$(ODIR)/%.o: FileLocation/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $< -lfl

$(ODIR)/%.o : CodeGeneration/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $< -lfl

$(ODIR)/%.o : Node/%.cpp 
	$(CC) $(CFLAGS) -c -o $@ $< -lfl





clean: 
	rm -f $(ODIR)/*.o
	rm -f lex.yy.c 
	rm -f scanner
	rm -f compile
	rm -f y.tab.c
	rm -f y.output
	rm -f y.tab.h

.PHONY: clean all
