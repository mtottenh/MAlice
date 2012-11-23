#include <cstring>
#include "TypeDefs.hpp"
#include "NStringLit.hpp"

NStringLit::NStringLit(char* literal)
{
	/* Remove the enclosing quotation marks from the string. */
	string temp = strdup(literal);
	this->literal = temp.substr(1, temp.size() - 1);
	
	this->type = resolveType();
}

int NStringLit::resolveType() {
	return TSTRING;
}
