#include <cstring>
#include "NStringLit.hpp"
#include "TypeDefs.hpp"
#include "../CodeGeneration/ASTVisitor.hpp"

NStringLit::NStringLit(char* literal) {
	/* Remove the enclosing quotation marks from the string. */
	string temp = literal;	

	this->literal = temp.substr(1, temp.size() - 1);
	nodeType = STRINGLIT;	
	this->type = resolveType();
	this->name = literal;
	this->weight = -1;
	if (name.find("\\n") != -1)
		name.replace(name.find("\\n"), 2, "\", 0xA, \"");
}

NStringLit::NStringLit(string s) {
	literal = s;
	name = s;
	type = resolveType();
}

int NStringLit::resolveType() {
	return TSTRING;
}

void NStringLit::accept(ASTVisitor *v) {
    v->visit(this);
}

int NStringLit::calculateWeight() {
	/* String is a primitive type; the weight should be 1. */
	return 1;
}
