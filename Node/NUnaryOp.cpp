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
	if(children[0]->getType() != TNUMBER) {
		return INVALIDTYPE;
	}
	else {
		return TNUMBER;
	}
}

int NUnaryOp::check() {
	this->type = resolveType();

	/* Valid if the type is a number and the children are valid. */
	return (type == TNUMBER) && Node::check();
}

#endif
