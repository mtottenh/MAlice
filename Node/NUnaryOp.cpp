#ifndef ___NUNARYOP__
#define ___NUNARYOP__

#include "NUnaryOp.hpp"

NUnaryOp::NUnaryOp(int op, Node* exp) { 
	name = "UnaryOp"; 
	this->op = op;
	children.push_back(exp);
}

#endif
