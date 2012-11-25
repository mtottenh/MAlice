#include "NArrayAccess.hpp"
#include "TypeDefs.hpp"
#include "../Errors/TypeMap.hpp" /* debug include, remove this! */

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
	case REFCHAR:
		return TCHAR;
		break;
	case REFNUMBER:
		return TNUMBER;
		break;
	case REFSTRING:
		return TSTRING;
		break;
	default:
		return INVALIDTYPE;
		break;
	}
}

int NArrayAccess::check() {
	int isValid = 1;
	
	cerr << "NArrayAccess::check() called!" << endl;
	this->type = resolveType();
	
	/* Does the identifier exist in scope? */
	Node* nodePtr = table->lookup(name);
	if(nodePtr == NULL) {
		error_var_not_found(name);
		isValid = 0;
	}

	/* Is the identifier an array type? */
	else {
		int nodeType = nodePtr->getType();
 
		if(nodeType != REFNUMBER && nodeType != REFCHAR 
				&& nodeType != REFSTRING) {
			error_not_array(nodePtr->getID(), nodeType);
			isValid = 0;
		}
	}

	/* Is the index node valid? Call node superclass. */
	isValid &= Node::check();

	return isValid;
}
