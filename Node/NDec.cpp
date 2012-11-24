#include "NDec.hpp"
#include "TypeDefs.hpp"

NDec::NDec(Node* child)
{
	name = "Decrement";
	children.push_back(child);
}

int NDec::resolveType() {
	int childType = children[0]->getType();

	if(childType == TNUMBER || childType == REFNUMBER) {
		return childType;
	}
	else {
		return INVALIDTYPE;
	}
}

int NDec::check() {
	this->type = resolveType();

	/* Is this the expected type (number)? */
	int isValid = (type == TNUMBER || type == REFNUMBER);
	
	/* Is the child valid? */
	isValid &= children[0]->check();

	return isValid;
}
