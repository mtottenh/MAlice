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
	Node *statlist; 
	isValid &= Node::check();
	int statNum = children[0]->getChildrenRef()->size();
	if (statNum > 0) {
		statlist = children[0]->getChildrenRef()->back();
	} else {
		if (nodeType == FUNC) {
			cerr << "No statements in function body!" << endl;
			isValid = 0;
			return isValid;
		}
	}
	if (statNum > 0) {
		std::deque<Node *> returnList = returnNodeList(statlist); 
		unsigned int size = returnList.size();

		if (nodeType == FUNC && isValid) {
			if (size == 0) {
				isValid = 0;
			}
		/* loop through the list and ensure that getType() of nReturn node == type*/
			for (unsigned int i = 0; i < size; i++) {
				if( !(returnList[i]->getType() == this->type)) {
					isValid = 0;
					cerr << "Type mismatch in function return statment!" << endl;
			}
		}
	} else {
		isValid &= (size == 0);
	}
	
	if (!isValid) {
		if (nodeType == FUNC)
			cerr << "Function does not contain a return statement" << endl;
		else if (nodeType == PROCEDURE && size > 0)
			cerr << "Procedure contains a return statement" << endl;
	}


}
	return isValid;
}
std::deque<Node *>  NFunctionDeclaration::returnNodeList(Node* statlist) {
	
	const std::deque<Node *>* statements = statlist->getChildrenRef();
	unsigned int size = statements->size();
	std::deque<Node *> returnList, leftList, rightList, conditionalQ;
	unsigned int sizeRight, sizeLeft;
	Node *left, *right;
	if ((*statements)[0] == NULL) {
		return returnList;
	}
	for (unsigned int i = 0; i < size; i++ ) {
		switch((*statements)[i]->getNodeType()) {
			case RETURN:
				returnList.push_back((*statements)[i]);
				break;
			case CONDITIONAL:
				/* Left branch always has nodetype STATMENTLIST*/
				left = (*statements)[i]->getChildren()[1];
				leftList = returnNodeList(left);
				sizeLeft  = leftList.size();
				for( unsigned int j = 0; j < sizeLeft; j++) {
					returnList.push_back(leftList[j]);
				}

				/* right could have nodetype CONDITIONAL!*/
				
				right = (*statements)[i]->getChildren()[2];
				if (right->getNodeType() == STATLIST) {

					rightList = returnNodeList(right);
					sizeRight = rightList.size();
					for( unsigned int j = 0; j < sizeLeft; j++) {
						returnList.push_back(rightList[j]);
					}
				} 
				if (right->getNodeType() == CONDITIONAL) {
					conditionalQ.push_back(right);
					while(!conditionalQ.empty()) {
						Node* temp = conditionalQ.front();
						conditionalQ.pop_front();
						if (temp->getNodeType() == CONDITIONAL) {
							/* Left branch always has nodetype STATMENTLIST*/
							left = temp->getChildren()[1];
							leftList = returnNodeList(left);
							sizeLeft  = leftList.size();
							for( unsigned int j = 0; j < sizeLeft; j++) {
								returnList.push_back(leftList[j]);
							}
							conditionalQ.push_back(temp->getChildren()[2]);
						}
					}
					/* FIGURE OUT WTF TO DO*/
				}
				/* LOLOLOLOLOLOLOL */
				break;
			case LOOP:
			case CODEBLOCK:
				if ((*statements)[i]->getChildren().size() > 0)
					left = (*statements)[i]->getChildren().back();
				leftList = returnNodeList(left);
				sizeLeft =  leftList.size();
				for( unsigned int j = 0; j < sizeLeft; j++) {
					returnList.push_back(leftList[j]);
				}
				break;
		}
	
	}
	return returnList;
}
/* TODO  finish check function */

#endif
