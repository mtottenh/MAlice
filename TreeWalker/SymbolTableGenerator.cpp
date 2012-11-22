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
	if (numberOfChildren > 1) {
		/* if we have a function that takes paramaters 
		 * let the codeblock create the new scope
		 * and then pass that to the Params node
		 */
		cout << "we got some paramaters\n";
		sym = nodeTableGen(children[0],sym);
		nodeTableGen(children[1],sym);
	} else {
		cout << "Number of children: " << numberOfChildren << endl;
		cout << "Child Type: " << children[0]->getNodeType();
		/*If it takes no args just process its child with a new scope */
		sym->print();
		sym = new SymbolTable(sym);
		nodeTableGen(children[0],sym);

		
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
/* Augments the given AST starting at
 * root with a symbol table
 * containing all identifiers from variable and function declarations
 */
SymbolTable* SymbolTableGenerator::generateTable() {
	SymbolTable* sym = new SymbolTable();
	while(!processQueue.empty()) {
		root = pop_front_q();
		cout << "---Generating symbol table for node:---\n";
		root->print();

		sym = nodeTableGen(root,sym);

		cout << "---Table Generated: --";
		sym->print();
		cout << "--end--\n";
	}
	return sym;
}
/* Takes an AST Beginning at node and generates a symbolTable Hierarchy
 * from that node onwards
 */
SymbolTable* SymbolTableGenerator::nodeTableGen(Node *node, SymbolTable* sym) {
	/* Add a pointer to the symbol table that the node is declared in*/
	if(!node->addTable(sym)) {
		cout << "VILLAGES HAVE BEEN BURNT RUN FOR YOUR LIVES\n";	
	}
		
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
	cout << "\nType in symbol table builder: " << type << endl;
	if ((type ==  FUNC) ||  (type == PROCEDURE)) {
		cout << "Generating table for Function Declaration: " << type << endl;
		sym = funcGen(node,sym);
//		sym->print();
	}
	
	/* If it is a variable declaration node then just add it 
	 * to the table and deal with the rest of the queue...
	 */
	if (type == VARDEC) {
		/*check if  identifier exists in this scope */
		cout << "Generating table for Variable Declaration: " << type << endl;
		Node* nodePtr = sym->lookupCurrentScope(node->getID());
		if (nodePtr != NULL) {
			error_var_exists(node->getID());
			return sym;

		}
		/* If it doesnt add it to the symbl table*/
		sym->add(node->getID(), root);	
		sym->print();
	}		
	if (type == CODEBLOCK) {
		/* if we are processing a codeblock 
		 * then create a new scope
		 * and add the children to the font of th equeu
		 */
		cout << "Generating table for CodeBlock: " << type << endl;
		sym = new SymbolTable(sym);
		for (unsigned int i = 0; i < node->getChildren().size(); i++) {
			/* Like dat bad coding bro? */
			nodeTableGen(node->getChildren()[i],sym);
		}

	}
	return sym;

}


