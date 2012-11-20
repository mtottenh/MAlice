#ifndef ___NCONDITIONAL__
#define ___NCONDITIONAL__

#include "NConditional.hpp"

NConditional::NConditional(Node* predicate, Node* left, Node* right) {
	name = "Conditional";
	children.push_back(predicate);
	children.push_back(left);
	children.push_back(right);
}

#endif
