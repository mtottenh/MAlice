#ifndef ___NSTATEMENTLIST__
#define ___NSTATEMENTLIST__

#include "NStatementList.hpp"
#include "TypeDefs.hpp"
NStatementList::NStatementList() 
{ 
	name = "StatList"; 
	nodeType = CODEBLOCK;
}

NStatementList::NStatementList(Node *dec, Node *ass) {
	name = "Dec&Assign";
	children.push_back(dec);
	children.push_back(ass);
	nodeType = CODEBLOCK;
}

int NStatementList::check() {
	int isValid = 1;
	
	/* If we have no children, then we have nothing to check! */
	if(children.size() == 0) {
		return isValid;
	}

	/*
	 * Check that the declaration child and assignment child have the same
	 * type.
	 */
	int t1 = children[0]->getType(); /* Type of declaration node */
	int t2 = children[1]->getType(); /* Type of assignment node */

	if(t1 != t2) {
		error_type_mismatch(children[0]->getID(), t1, t2);
		isValid = 0;
	}

	/* Check the children via the Node superclass. */
	isValid &= Node::check();

	return isValid;
}

#endif
