#include "../CodeGeneration/ASTVisitor.hpp"

#include "NReturn.hpp"
#include "TypeDefs.hpp"
NReturn::NReturn(Node* exp)
{
	name = "Return";
	children.push_back(exp);
	nodeType = RETURN;
}

int NReturn::resolveType()
{
	this->type = children[0]->getType();
	return this->type;
}


void NReturn::accept(ASTVisitor *v) {
    v->visit(this);
}
