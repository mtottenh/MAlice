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
	this->type = resolveType();

	/* Is this the expected type (number)? */
	int isValid = (type == TNUMBER || type == REFNUMBER);
	
	/* Is the child valid? */
	isValid &= children[0]->check();

	return isValid;
}
