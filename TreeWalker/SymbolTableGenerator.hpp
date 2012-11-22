#ifndef _SYMBOLTABLEGEN_H_
#define _SYMBOLTABLEGEN_H_
#include "../Node/Node.hpp"
#include "../SymbolTable/SymbolTable.hpp"

class SymbolTableGenerator {
private:
	Node *root;
	SymbolTable* sym;
	SymbolTable* funcGen(Node*);
	std::deque<Node *> ProcessQueue;
	Node* pop_front_q();
	SymbolTable* init() {return NULL;};
public:
	SymbolTableGenerator(Node*);
	SymbolTable* generateTable(Node*);
	
}
#endif

