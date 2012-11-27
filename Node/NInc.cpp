#include "NInc.hpp"
#include "TypeDefs.hpp"

NInc::NInc(Node* child) {
	name = "Increment"; 
	children.push_back(child);
}

int NInc::resolveType() {
	int childType = children[0]->getType();

	if(childType == TNUMBER || childType == REFNUMBER) {
		return childType;
	}
	else {
		return INVALIDTYPE;
	}
}

int NInc::check() {
	int isValid;
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
