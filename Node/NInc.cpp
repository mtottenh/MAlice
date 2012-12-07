#include "../CodeGeneration/ASTVisitor.hpp"
#include "NInc.hpp"
#include "TypeDefs.hpp"

/* Constructors. */

NInc::NInc(Node* child) {
	this->name = "Increment"; 
	children.push_back(child); /* Push the node that is to be incremented. */
	this->nodeType = INC;
}

/* Public methods. */

int NInc::check() {
	int isValid;

	/* Get the type of the node. */
	this->type = resolveType();

	/* Is this the expected type (number)? */
	if(type != TNUMBER && type != REFNUMBER) {
		printErrorHeader("increment");
		error_type_mismatch(INC, children[0]->getType(), TNUMBER);
		isValid = 0;
	}
	
	/* Is the child valid? */
	isValid &= children[0]->check();

	return isValid;
}

/* Protected methods. */

int NInc::resolveType() {
	/* Get the child's type. */
	int childType = children[0]->getType();

	/* Is it a number or a reference to a number? If so, this is very good. */
	if(childType == TNUMBER || childType == REFNUMBER) {
		return childType;
	}

	/* ... this isn't really good. Invalid type. */
	else {
		return INVALIDTYPE;
	}
}


void NInc::accept(ASTVisitor *v) {
    v->visit(this);
}
