#ifndef ___NUNARYOP__
#define ___NUNARYOP__

#include "NUnaryOp.hpp"
#include "TypeDefs.hpp"

NUnaryOp::NUnaryOp(int op, Node* exp) { 
	name = "UnaryOp"; 
	this->op = op;
	children.push_back(exp);
}

int NUnaryOp::resolveType() {
	/* Is the type of the expression a number? */
	int childType = children[0]->getType();

	if(childType != TNUMBER && childType != REFNUMBER) {
		return INVALIDTYPE;
	}
	else {
		return TNUMBER;
	}
}

int NUnaryOp::check() {
	this->type = resolveType();

	/* Valid if the type is a number and the children are valid. */
	return (type == TNUMBER || type == REFNUMBER) && Node::check();
}

#endif
