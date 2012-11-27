#include "NConditional.hpp"
#include "TypeDefs.hpp"
/* 
 * Conditionals consist of a predicate a left branch and a right branch
 */

/* Constructors. */

NConditional::NConditional(Node* predicate, Node* left, Node* right) {
	this->name = "Conditional";
	this->nodeType = CONDITIONAL;
	children.push_back(predicate); /* Expression representing condition. */
	children.push_back(left); /* Push the LHS (then) case. */
	children.push_back(right); /* Push the RHS (else/elseif/endif) case. */
}

/* Public methods. */

int NConditional::check() {
	int isValid = 1;

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

/* Protected methods. */

int NConditional::resolveType() {
	/* A conditional doesn't have a type! */
	return INVALIDTYPE;
}
