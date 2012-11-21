#ifndef _SYMBOLTABLEGEN_H_
#define _SYMBOLTABLEGEN_H_
#include "../Node/Node.hpp"
#include "../SymbolTable/SymbolTable.hpp"

class SymbolTableGenerator {
private:
	Node *root;
	SymbolTable* sym;
public:
	SymbolTableGenerator(Node *);
	SymbolTable* generateTable();
	
}
#endif

