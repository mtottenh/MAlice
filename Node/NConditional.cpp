#ifndef ___NCONDITIONAL__
#define ___NCONDITIONAL__

#include "NConditional.hpp"
#include "TypeDefs.hpp"
/* 
 * Conditionals consist of a predicate a left branch and a right branch
 */
NConditional::NConditional(Node* predicate, Node* left, Node* right) {
	this->name = "Conditional";
	children.push_back(predicate); /* This is an expression */
	children.push_back(left);
	children.push_back(right);
}

int NConditional::resolveType() {
	return INVALIDTYPE;
}

int NConditional::check() {
	int isValid = 1;

	this->type = resolveType(); /* TODO Why is this even needed? */

	/* Is the predicate node actually a predicate (of type boolean?) */
	int predNodeType = children[0]->getType();

	if(predNodeType != BOOLEAN) {
		printErrorHeader("conditional");
		error_type_mismatch_cond(predNodeType);
		isValid = 0;
	}

	/* Check that all nodes are valid (using superclass check) */
	isValid &= Node::check();

	return isValid;
}

#endif
