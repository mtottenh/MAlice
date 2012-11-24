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
	int numberOfChildren = func->getChildren().size();
	std::deque<Node *> children = func->getChildren();
	sym->add(func->getID(),func);
	Node* Codeblock, Declist, Statlist, Paramlist;

	if (numberOfChildren > 1) {
		/* if we have a function that takes paramaters 
		 * let the codeblock create the new scope
		 * and then pass that to the Params node
		 */
		Codeblock = children[0];
		Paramlist = children[1];
			
		sym = nodeTableGen(children[0],sym);

		nodeTableGen(children[1],sym);
	} else {
//		cout << "Number of children: " << numberOfChildren << endl;
//		cout << "Child Type: " << children[0]->getNodeType();
		/*If it takes no args just process its child with a new scope */
//		sym->print();
		sym = new SymbolTable(sym);
		nodeTableGen(children[0],sym);

		
//		cout << "Generating table for CodeBlock: " << type << endl;
	}
	
	return sym;

}
/* Wapper for getting the fisrt elem of a queue then
 * deleting that element from the queue
 */
Node* SymbolTableGenerator::pop_front_q() {
	Node* front = processQueue.front();
	processQueue.pop_front();
	return front;
}
/*
 * Augments the given AST starting at root with a symbol table,
 * containing all identifiers from variable and function declarations.
 */
SymbolTable* SymbolTableGenerator::generateTable() {
	SymbolTable* sym = new SymbolTable();
	root->addTable(sym);
	while(!processQueue.empty()) {
		root = pop_front_q();
		sym = nodeTableGen(root,sym);
	}
	return sym;
}

/* Takes an AST starting at node and generates a symbol table hierarchy
 * from that node onwards.
 */
SymbolTable* SymbolTableGenerator::nodeTableGen(Node *node, SymbolTable* sym) {
	/* Add a pointer to the symbol table that the node is declared in. */
	if(!node->addTable(sym)) {
		cout << "ERROR: Could not add symbol table to node!" << endl;	
	}
	
	int type = node->getNodeType();

	/*
	 * If the node is a function declaration or procedure declaration, 
	 * delegate to the funcGen function.
	 */
	if ((type ==  FUNC) ||  (type == PROCEDURE)) {
		sym = funcGen(node,sym);
	}
	
	/*
	 * If it is a variable declaration node, add its ID to the symbol
	 * table. Checking that the variable is not already declared is a job
	 * for semantic analysis, so is not done here.
	 */
	if (type == VARDEC) {
		/*check if  identifier exists in this scope */
		Node* nodePtr = sym->lookupCurrentScope(node->getID());
		if (nodePtr != NULL) {
			error_var_exists(node->getID());
			return sym;

		}
		/* If it doesnt add it to the symbl table*/
		sym->add(node->getID(), node);	
	}		
	if (type == CODEBLOCK) {
		/* if we are processing a codeblock 
		 * then create a new scope
		 * and add the children to the font of th equeu
		 */
		sym = new SymbolTable(sym);
		if (node->getChildren().size() > 1) {
		/* If we consists of more than just a dec list we 
		 * need to propagate the symbol table to the statlist
 		 */
			
		}
		for (unsigned int i = 0; i < node->getChildren().size(); i++) {
			/* Like dat bad coding bro? */
			/* no.jpg */
			sym = nodeTableGen(node->getChildren()[i],sym);
		}
		return sym;
	}
	if (type == DECLARATIONBLOCK) {
		for (unsigned int i = 0; i < node->getChildren().size(); i++) {
			/* Like dat bad coding bro? */
			/* no.jpg */
			nodeTableGen(node->getChildren()[i],sym);
		}
		return sym;	
	}
	for (unsigned int i = 0; i < node->getChildren().size(); i++) {
		/* Like dat bad coding bro? */
		/* no.jpg */
		nodeTableGen(node->getChildren()[i],sym);
	}
	
	
	return sym;
}


