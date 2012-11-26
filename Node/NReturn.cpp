#include "NReturn.hpp"
NReturn::NReturn(Node* exp)
{
	name = "Return";
	children.push_back(exp);
}

int NReturn::resolveType()
{
	return this->type = children[0]->getType();
}
