#include "NArrayAccess.hpp"
#include "TypeDefs.hpp"

NArrayAccess::NArrayAccess(NIdentifier* id, Node* indexNode)
{
	/* Do we need this identifier? */
	this->id = id;
	name = id->getID();
	this->indexNode = indexNode;
}

int NArrayAccess::check() {
	int isValid = 1;
	
	/* Does the identifier exist in scope? */
	Node* nodePtr = table->lookup(name);
	if(nodePtr == NULL) {
		error_var_not_found(name);
		isValid = 0;
	}

	/* Is the identifier a keyword? */
	else if(nodePtr->getType() == KEYWORD) {
		error_keyword(name);
		isValid = 0;
	}

	/* Is the index node valid? */
	isValid &= indexNode->check();

	return isValid;
}
