#ifndef ___NFUNCTIONDECLARATION__
#define ___NFUNCTIONDECLARATION__

#include "NFunctionDeclaration.hpp"
#include "TypeDefs.hpp"

NFunctionDeclaration::NFunctionDeclaration() { 
	name = "FuncDec"; 
}                            

/* Declaring a procedure with no arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, Node *block) {
        this->type = VOID;
	children.push_back(block);
	/* change behaviour */
	name = ID->getID(); 
	nodeType = PROCEDURE;
	delete(ID);
}                                                                        
/* Delcaring Function with no arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, Node *block, int type) {
	this->type = type;
	name = ID->getID();
	children.push_back(block);
	nodeType = FUNC;
	delete(ID);
}

/* declaring a function with arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, Node *params, 
					    Node *block, int type) {
	this->type = type;
	name = ID->getID();
	delete(ID);
	children.push_back(block);
	children.push_back(params);
	nodeType = FUNC;
}

/* Declaring a procedure with arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, Node *params,Node *block) {
	this->type = VOID;
	name = ID->getID();
	delete(ID);
	children.push_back(block);
	children.push_back(params);
	nodeType = PROCEDURE;
}


int NFunctionDeclaration::print() const {
	cout << name ;                                                   
	return 1;                                                        
}
int NFunctionDeclaration::getType() const {
	return type;
}

int NFunctionDeclaration::check() {
	int isValid = 1;

	Node* nodePtr = table->lookup(name);
	/* This check is performed int he symbol table generator*/
	/* Does the identifier exist in local scope? */
/*	if(nodePtr != NULL) {
		error_var_exists(name);
		isValid = 0;
	}*/
	/* Check the function code block and arguments. */
	isValid &= Node::check(); 

	return isValid;
}

#endif
