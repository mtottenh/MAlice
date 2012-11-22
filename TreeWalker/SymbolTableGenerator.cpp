#include "SymbolTableGenerator.hpp"
#include "../Errors/SemanticErrors.hpp"
#include "../Node/TypeDefs.hpp"

SymbolTableGenerator::SymbolTableGenerator(Node *tree) {
	root = tree;
 	processQueue = root->getChildren();	
}

/* The contract for this is that it generates a symbol table for a function 
 * and returns a poitner to it. 
 * The process queue's state after this function has been run
 * should just have the first element (The function) and any elements it adds
 * removed
 */
SymbolTable* SymbolTableGenerator::funcGen(Node *func, SymbolTable* sym) {
	/*get funcs children and add them to the front of the queue*/
	addChildren(func);	
	/*check if  identifier exists in this scope */
	Node* nodePtr = lookupCurrentScope(func.getID());
	if (nodePtr != NULL) {
		error_var_exists(func.getID());
		return sym;

	}
	/* If it doesnt add it to the symbl table*/
	sym.add(root.getID(), root);

	
	/* Function declarations create a new scope block for their
 	 * paramaters and function body
	 */
	while (!processQueue.empty()) {
		SymbolTable* t_sym = new SymbolTable(sym);
		func = pop_fron_q();
		if (type == CODEBLOCK) {
			/* add the codeblocks children to the front of the queue*/
			/* process them with t_sym as the symbol table*/
		
		} else {
			generateTable();
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
SymbolTable* SymbolTableGenerator::generateTable(Node *node, SymbolTable* sym) {
	/* Add a pointer to the symbol table that the node is declared in*/
	node.addTable(sym);
		
	int type = node->getNodeType();
	/* Check the type of the Node, if it is not a Declaration
         * Or paramter declaration then do not add its ID
  	 * to the symbol table but just update the node with the
	 * symbol table and  create a new scope if
	 * nessecary	
	 */

	/* If it is a Func Declaration node add it to the symbol table
	 * and deal with the nodes children
	 */
	if (type == ( FUNC || PROCEDURE)) {
		funcGen(node,sym);
	}
	
	/* If it is a variable declaration node then just add it 
	 * to the table and deal with the rest of the queue...
	 */
	if (type == VARDEC) {
		/*check if  identifier exists in this scope */
		Node* nodePtr = lookupCurrentScope(func.getID());
		if (nodePtr != NULL) {
			error_var_exists(node.getID());
			return sym;

		}
		/* If it doesnt add it to the symbl table*/
		sym.add(node.getID(), root);	
	}		
	if (type == CODEBLOCKK) {
		/* if we are processing a codeblock 
		 * then create a new scope
		 * and add the children to the font of th equeu
		 */
	}
	/* once we have dealt with the node deal with the rest of the process queue*/
	while (!processQueue.empty()) {
		node = pop_front_q();
	 	generateTable(node,sym);
	}
	return sym;

}


