#include <cstring>
#include "NStringLit.hpp"
#include "TypeDefs.hpp"


NStringLit::NStringLit(char* literal)
{
	/* Remove the enclosing quotation marks from the string. */
	string temp = strdup(literal);
	this->literal = temp.substr(1, temp.size() - 1);
	nodeType = STRINGLIT;	
	this->type = resolveType();
	this->name = literal;

}
/* TODO check to see if we need to remove the " " from s*/
NStringLit::NStringLit(string s) {
	literal = s;
	name = s;
	type = resolveType();
}
int NStringLit::resolveType() {
	return TSTRING;
}
