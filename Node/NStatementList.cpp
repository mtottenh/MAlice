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

#endif
