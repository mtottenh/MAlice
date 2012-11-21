#include "SymbolTableGenerator.hpp"
#include "../Errors/SemanticErrors.hpp"

SymbolTableGenerator::SymbolTableGenerator(Node *tree) {
	root = tree;
	sym = new SymbolTable();		
	
}

SymbolTable* SymbolTableGenerator::generateTable(Node *root) {
	std::vector<Node *> processQueue = root->getChildren();
	int numberOfChildren = children.size();
	int type = root.getType();
	/* Check the type of the Node, if it is not a Declaration
         * Or paramter declaration then do not add its ID
  	 * to the symbol table but just update the node with the
	 * symbol table and  create a new scope if
	 * nessecary	
	 */

	/* Add a pointer to the symbol table */
	root.addTable(sym);

	/* Fix by using acutal typenames later */
	if (type == ( FUNCDEC || PROCEDUREDEC || VARDEC || PARAMDEC)) {
		/*check if  identifier exists in this scope */
		Node* nodePtr = lookupCurrentScope(root.getID());
		if (nodePtr != NULL) {
			error_var_exists(root.getID();
			return sym;

		}
		/* Look up in all scopes*/
		nodePtr = lookup(root.getID());
		/*if it is not a keyword add it*/
		if(nodePtr != NULL) {
			if (nodePtr->getType() != KEYWORD) {
				sym.add(root.getID(),root);
			} else {
				error_keyword(root.getID());
				return sym;
			}
		}
	}

	/* if we hit a new codeblock create a new scope */
	if (type == CODEBLOCK ) {
		sym = new SymbolTable(sym);
	} 

	 
	while (!processQueue.empty()) {
		generateTable(children.pop());
	}
	return sym;
	
}

