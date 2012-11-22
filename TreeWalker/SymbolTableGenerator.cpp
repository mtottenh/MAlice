#include "SymbolTableGenerator.hpp"
#include "../Errors/SemanticErrors.hpp"
#include "../Node/TypeDefs.hpp"

SymbolTableGenerator::SymbolTableGenerator(Node *tree) {
	root = tree;
	sym = new SymbolTable();		
	
}
SymbolTable* SymbolTableGenerator::funcGen(Node *root) {
	return NULL
}
SymbolTable* SymbolTableGenerator::generateTable(Node *root) {
	std::vector<Node *> processQueue = root->getChildren();
	int numberOfChildren = children.size();
	int type = root.getNodeType();
	/* Check the type of the Node, if it is not a Declaration
         * Or paramter declaration then do not add its ID
  	 * to the symbol table but just update the node with the
	 * symbol table and  create a new scope if
	 * nessecary	
	 */

	/* Add a pointer to the symbol table */
	root.addTable(sym);

	/* If it is a Declaration Add to the symbol table */
	if (type == ( FUNC || PROCEDURE)) {
		/*check if  identifier exists in this scope */
		Node* nodePtr = lookupCurrentScope(root.getID());
		if (nodePtr != NULL) {
			error_var_exists(root.getID();
			return sym;

		}
		/* Look up in all scopes*/
		nodePtr = lookup(root.getID());
		/*if it is not a keyword or hasn't been used add it*/
		if(nodePtr != NULL) {
			if (nodePtr->getType() != KEYWORD) {
				sym.add(root.getID(),root);
			} else {
				error_keyword(root.getID());
				return sym;
			}
		} else {
		
			sym.add(root.getID(), root);
		}

		/* Function declarations create a new scope block for their
 		 * paramaters and function body
		 */
		sym = new SymbolTable(sym);
	}

	/* if we hit a new codeblock create a new scope  PROBLEM HERE WITH FUNCS*/
	if (type == CODEBLOCK ) {
		sym = new SymbolTable(sym);
	} 

	 
	while (!processQueue.empty()) {
		generateTable(children.pop());
	}
	return sym;
	
}



