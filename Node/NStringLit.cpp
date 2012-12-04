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
    cerr << "Node: String Literal" << endl;
    v->visit(this);
}

int NStringLit::calculateWeight() {
	/* String is a primitive type; the weight should be 1. */
	return 1;
}
