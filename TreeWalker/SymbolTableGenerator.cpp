#include "SymbolTableGenerator.hpp"
#include "../Errors/SemanticErrors.hpp"
#include "../Node/TypeDefs.hpp"

SymbolTableGenerator::SymbolTableGenerator(Node *tree) {
	root = tree;
	sym = new SymbolTable();		
 	processQueue = root->getChildren();	
}

/* The contract for this is that it generates a symbol table for a function 
 * and returns a poitner to it. THIS MUST NOT EDIT THE SYM POINTER
 * The process queue's state after this function has been run
 * should just have the first element (The function) and any elements it adds
 * removed
 */
SymbolTable* SymbolTableGenerator::funcGen(Node *func, std::vector<Node*> pq) {
	/*check if  identifier exists in this scope */
	Node* nodePtr = lookupCurrentScope(func.getID());
	if (nodePtr != NULL) {
		error_var_exists(func.getID();
		return sym;

	}
	/* If it doesnt add it to the symbl table*/
	sym.add(root.getID(), root);

	
	/* Function declarations create a new scope block for their
 	 * paramaters and function body
	 */
	while (!pq.empty()) {
		root = pq.pop()
		if (type == CODEBLOCK) {
			/* go straight to generating code for its children */
		} else {
			generateTable(pq.pop());
		}
	}
	
	return new SymbolTable(sym);

}

Node* SymbolTableGenerator::pop_front_q() {
	Node* front = processQueue.front();
	processQueue.pop_front();
	return front;
}
/* Augments the given AST starting at Node with a symbol table
 * containing all identifiers from variable and function declarations
 */
SymbolTable* SymbolTableGenerator::generateTable(Node *node) {
	/* Add a pointer to the symbol table */
	node.addTable(sym);

	int type = node->getNodeType();
	/* Check the type of the Node, if it is not a Declaration
         * Or paramter declaration then do not add its ID
  	 * to the symbol table but just update the node with the
	 * symbol table and  create a new scope if
	 * nessecary	
	 */

	/* If it is a Func Declaration Add to the symbol table*/
	if (type == ( FUNC || PROCEDURE)) {
		funcGen(node);
		while (!processQueue.empty()) {
			node = pop_front_q;
		 	generateTable(node);
		}
		return sym;
		
	}
	/* If it is a variable declaration then just add it 
	 * to the table
	 */
	if (type == VARDEC {
		
	}
	

	return sym;
	
}



