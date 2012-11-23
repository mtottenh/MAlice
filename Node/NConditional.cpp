#ifndef ___NCONDITIONAL__
#define ___NCONDITIONAL__

#include "NConditional.hpp"
#include "TypeDefs.hpp"

NConditional::NConditional(Node* predicate, Node* left, Node* right) {
	this->name = "Conditional";
	children.push_back(predicate);
	children.push_back(left);
	children.push_back(right);
}

int NConditional::resolveType() {
	return INVALIDTYPE;
}

int NConditional::check() {
	int isValid = 1;

	this->type = resolveType();

	/* Is the predicate node actually a predicate (of type boolean?) */
	int predNodeType = children[0]->getType();

	if(predNodeType != BOOLEAN) {
		error_type_mismatch_cond(predNodeType);
		isValid = 0;
	}

	/* Check that all nodes are valid (using superclass check) */
	isValid &= Node::check();

	return isValid;
}

#endif
