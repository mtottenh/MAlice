#include "NDec.hpp"
#include "TypeDefs.hpp"

/* Constructors. */

NDec::NDec(Node* child) {
	this->name = "Decrement";
	children.push_back(child); /* Push the node that is to be decremented. */
	this->nodeType = DEC;
}

/* Public methods. */

int NDec::check() {
	int isValid;

	/* Get the type of the node. */
	this->type = resolveType();

	/* Is this the expected type (number)? */
	if(type == INVALIDTYPE) {
		printErrorHeader("decrement");
		error_type_mismatch(DEC, children[0]->getType(), TNUMBER);
		isValid = 0;
	}
	
	/* Is the child valid? */
	isValid &= children[0]->check();

	return isValid;
}

/* Protected methods. */

int NDec::resolveType() {
	/* Get the child's type. */
	int childType = children[0]->getType();

	/* Is it a number or a reference to a number? If so, this is good. :) */
	if(childType == TNUMBER || childType == REFNUMBER) {
		return childType;
	}

	/* ... or not good. Invalid type! */
	else {
		return INVALIDTYPE;
	}
}
