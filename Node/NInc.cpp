#include "NInc.hpp"
#include "TypeDefs.hpp"

NInc::NInc(Node* child) {
	name = "Increment"; 
	children.push_back(child);
	this->type = resolveType();
}

int NInc::resolveType() {
	int childType = children[0]->getType();

	if(childType == TNUMBER) {
		return TNUMBER;
	}
	else {
		return INVALIDTYPE;
	}
}

int NInc::check() {
	/* Is this the expected type (number)? */
	int isValid = (type == TNUMBER);
	
	/* Is the child valid? */
	isValid &= children[0]->check();

	return isValid;
}
