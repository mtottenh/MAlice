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

/* TODO remove print and gettype
 * these should be inherited from Node anyway
 */
int NFunctionDeclaration::print() const {
	cout << name ;                                                   
	return 1;                                                        
}
int NFunctionDeclaration::getType() {
	return type;
}


int NFunctionDeclaration::check() {
	int isValid = 1;
	Node *statlist = children[0]->getChildren().back();
	std::deque<Node *> returnList = returnNodeList(statlist); 
	unsigned int size = returnList.size();

	if (nodeType == FUNC) {
		isValid &= (size > 0);
		/* loop through the list and ensure that getType() of nReturn node == type*/
		for (unsigned int i = 0; i < size; i++) {
			isValid &= returnList[i]->getType() == this->type;
		}
	} else {
		isValid &= (size == 0);
	}

	return isValid;
}
std::deque<Node *>  NFunctionDeclaration::returnNodeList(Node* statlist) {
	unsigned int size = statlist->getChildren().size();
	std::deque<Node *> statements = statlist->getChildren();
	std::deque<Node *> returnList;
	for (unsigned int i = 0; i < size; i++ ) {
		switch(statements[i]->getNodeType()) {
			case RETURN:
				returnList.push_back(statements[i]);
				break;
			case CONDITIONAL:
				/* LOLOLOLOLOLOLOL */
				break;
			case CODEBLOCK:
				break;
			case LOOP:
				break;
		}
	
	}
	return returnList;
}
/* TODO  finish check function */

#endif
