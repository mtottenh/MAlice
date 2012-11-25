#include "NArrayAccess.hpp"
#include "TypeDefs.hpp"

NArrayAccess::NArrayAccess(NIdentifier* id, Node* indexNode)
{
	this->name = id->getID();
	this->id = id;
	this->indexNode = indexNode;
	children.push_back(id);
	children.push_back(indexNode);
}

int NArrayAccess::getType() {
	type = resolveType();
	return type;
}

int NArrayAccess::resolveType() {
	/* Return type of the identifier, excluding the 'array' bit. */
	Node* nodePtr = table->lookup(name);
	if(nodePtr == NULL) {
		return INVALIDTYPE;
	}

	int idType = nodePtr->getType();

	switch(idType) {
	case ARRCHAR:
		return TCHAR;
		break;
	case ARRNUMBER:
		return TNUMBER;
		break;
	case ARRSTRING:
		return TSTRING;
		break;
	default:
		return INVALIDTYPE;
		break;
	}
}

int NArrayAccess::check() {
	int isValid = 1;

	this->type = resolveType();
	
	/* Does the identifier exist in scope? */
	Node* nodePtr = table->lookup(name);
	if(nodePtr == NULL) {
		error_var_not_found(name);
		isValid = 0;
	}

	/* Is the index node valid? Call node superclass. */
	isValid &= Node::check();

	return isValid;
}
