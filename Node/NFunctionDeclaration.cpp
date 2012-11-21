#ifndef ___NFUNCTIONDECLARATION__
#define ___NFUNCTIONDECLARATION__

#include "NFunctionDeclaration.hpp"
#include "TypeDefs.hpp"

NFunctionDeclaration::NFunctionDeclaration() { 
	name = "FuncDec"; 
}                            

/* Declaring a procedure with no arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, Node *block) {
        children.push_back(block);
	/* change behaviour */
	name = ID->getID(); 
	delete(ID);
}                                                                        
/* Delcaring Function with no arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, Node *block, int type) {
	this->type = type;
	name = ID->getID();
	children.push_back(block);
	delete(ID);
}

/* declaring a function with arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, Node *params, 
					    Node *block, int type) {
	this->type = type;
	name = ID->getID();
	delete(ID);
	children.push_back(params);
	children.push_back(block);
}

/* Declaring a procedure with arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, Node *params,Node *block) {
	name = ID->getID();
	delete(ID);
	children.push_back(params);
	children.push_back(block);
}


int NFunctionDeclaration::print() const {
	cout << name ;                                                   
	return 1;                                                        
}
int NFunctionDeclaration::getType() const {
	return type;
}

int NFunctionDeclaration::check(SymbolTable* table) {
	int isValid = 1;

	Node* nodePtr = table->lookup(name);

	/* Does the variable exist in scope? If not, error. */
	if(nodePtr == NULL) {
		error_var_not_found(name);
		isValid = 0;
	}

	/* Is the variable a keyword? If so, error. */
	else if (nodePtr->getType() == KEYWORD) {
		error_keyword(name);
		isValid = 0;
	}

	/* Check the function code block and arguments. */
	isValid &= Node::check(table); 

	return isValid;
}

#endif
