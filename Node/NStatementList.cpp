#include "NStatementList.hpp"
#include "TypeDefs.hpp"
#include "../CodeGeneration/ASTVisitor.hpp"
/* TODO Check grammar.y and see if this is needed */
NStatementList::NStatementList() 
{ 
	name = "StatList"; 
	nodeType = STATLIST;
}

NStatementList::NStatementList(Node *dec, Node *ass) {
	name = "Dec&Assign";
	children.push_back(dec);
	children.push_back(ass);
	nodeType = STATLIST;
}

int NStatementList::check() {
	int isValid = 1;
	
	/* If we have no children, then we have nothing to check! */
	if(children.size() == 0) {
		return isValid;
	}

	/* Check the children via the Node superclass. */
	isValid &= Node::check();

	return isValid;
}
void NStatementList::accept(ASTVisitor *v) {
    unsigned int numChildren = this->getChildrenSize();
    for(unsigned int i =0; i < numChildren; i++) {
        this->getChild(i)->accept(v);
    }
}
