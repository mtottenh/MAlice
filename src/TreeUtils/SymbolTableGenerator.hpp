#ifndef _SYMBOLTABLEGEN_H_
#define _SYMBOLTABLEGEN_H_
#include "../Node/Node.hpp"
#include "../SymbolTable/SymbolTable.hpp"
typedef boost::shared_ptr<SymbolTable> sym_table_ptr;
class SymbolTableGenerator {
private:
	Node *root;
	int funcGen(Node*, sym_table_ptr);
	std::deque<Node *> processQueue;
	Node* pop_front_q();
public:
	SymbolTableGenerator(Node*);
	int nodeTableGen(Node*,sym_table_ptr);
	int generateTable();
	
};
#endif

