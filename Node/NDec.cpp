#include "NDec.hpp"
#include "TypeDefs.hpp"

NDec::NDec(Node* child)
{
	name = child->name;
	children.push_back(child);
}

int NDec::resolveType() {
	int childType = children[0]->getType();

	if(childType == TNUMBER) {
		return TNUMBER;
	}
	else {
		return INVALIDTYPE;
	}
}

int NDec::check() {
	/* Is this the expected type (number)? */
	int isValid = (type == TNUMBER);
	
	/* Is the child valid? */
	isValid &= children[0]->check();

	return isValid;
}
