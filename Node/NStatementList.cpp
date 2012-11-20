#ifndef ___NSTATEMENTLIST__
#define ___NSTATEMENTLIST__
#include "NStatementList.hpp"

NStatementList::NStatementList(Node *dec, Node *ass) {
	name = "Dec&Assign";
	children.push_back(dec);
	children.push_back(ass);
}
#endif
