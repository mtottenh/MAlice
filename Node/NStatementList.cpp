#ifndef ___NSTATEMENTLIST__
#define ___NSTATEMENTLIST__

#include "NStatementList.hpp"
#include "TypeDefs.hpp"
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

#endif
