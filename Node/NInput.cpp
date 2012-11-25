#include "NInput.hpp"
#include "TypeDefs.hpp"

NInput::NInput(Node* id)
{
	name = "NInput";
	nodeType = INPUTNODE;
	children.push_back(id);
}

int NInput::check() {
	int isValid = 1;

	/* 
	 * We need to ensure that the identifier is a type that can accept
	 * input.
	 */
	int childType = children[0]->getType();

	if(childType != TNUMBER && childType != TCHAR && childType != TSTRING) {
		error_input_type(children[0]->getID(), childType);
		isValid = 0;
	}

	/* Check the children using Node superclass. */
	isValid &= Node::check();

	return isValid;
}
