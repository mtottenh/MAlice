#ifndef ___NFUNCTIONDECLARATION__
#define ___NFUNCTIONDECLARATION__

#include "NFunctionDeclaration.hpp"

NFunctionDeclaration::NFunctionDeclaration() { 
	name = "FuncDec"; 
}                            

/* Declaring a procedure with no arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, Node *block) {
        children.push_back(block);
	/* change behaviour */
	name = ID.getID(); 
	delete(ID);
}                                                                        
/* Delcaring Function with no arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, Node *block, int type) {
	this.type = type
	name = ID.getID();
	children.push_back(block);
	delete(ID);
}

/* declaring a function with arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, Node *params, 
					    Node *block, int type) {
	this.type = type
	name = ID.getID();
	delete(ID);
	children.push_back(params);
	children.push_back(block);
}

/* Declaring a procedure with arguments */
NFunctionDeclaration::NFunctionDeclaration (NIdentifier* ID, Node *params,Node *block) {
	name = ID.getID();
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

#endif
