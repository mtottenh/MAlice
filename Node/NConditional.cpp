#ifndef ___NCONDITIONAL__
#define ___NCONDITIONAL__

#include "NConditional.hpp"

NConditional::NConditional(Node* predicate, Node* left, Node* right) {
	name = "Conditional";
	children.push_back(predicate);
	children.push_back(left);
	children.push_back(right);
}

/* This node is appararently broken, check/resolve type functions are dummy
 * methods. Fix this! */

int NConditional::resolveType() {
	return 1;
}

int NConditional::check() {
	return 1;
}

#endif
