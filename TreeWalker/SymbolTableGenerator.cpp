#include "SymbolTableGenerator.hpp"
#include "../Errors/SemanticErrors.hpp"

SymbolTableGenerator::SymbolTableGenerator(Node *tree) {
	root = tree;
	sym = new SymbolTable();		
	
}

SymbolTable* SymbolTableGenerator::generateTable() {
	std::vector<Node *> processQueue = root->getChildren();
	int numberOfChildren = children.size();
	int type = root.getType();
	
	/*check if  identifier exists in this scope */
	Node* nodePtr = lookupCurrentScope(root.getID());
	if (nodePtr != NULL) {
		error_var_exists(root.getID();
		return sym;

	}
	/* Look up in all scopes*/
	nodePtr = lookup(root.getID());
	/*if it exists and is a keyword*/
	if(nodePtr != NULL) {
		if (nodePtr->getType() != KEYWORD) {
			sym.add(root.getID(),root);
		} else {
			error_keyword(root.getID());
			return sym;
		}
	}
		



	/* if we hit a new codeblock create a new scope */
	if (type ==  CODEBLOCK ) {
		sym = new SymbolTable(sym);
	} 
	while (!processQueue.empty())
		
	
}

