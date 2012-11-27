#ifndef _SYMBOLTABLEGEN_H_
#define _SYMBOLTABLEGEN_H_
#include "../Node/Node.hpp"
#include "../SymbolTable/SymbolTable.hpp"

class SymbolTableGenerator {
private:
	Node *root;
	void funcGen(Node*, SymbolTable*);
	std::deque<Node *> processQueue;
	Node* pop_front_q();
public:
	SymbolTableGenerator(Node*);
	void nodeTableGen(Node*,SymbolTable*);
	void generateTable();
	
};
#endif

