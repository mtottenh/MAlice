#include "NInput.hpp"
#include "TypeDefs.hpp"

/* Constructors. */

NInput::NInput(Node* id)
{
	this->name = "NInput";
	this->nodeType = INPUTNODE;
	children.push_back(id);
}

/* Public methods. */

int NInput::check() {
	int isValid = 1;

	/* 
	 * We need to ensure that the identifier is a type that can accept
	 * input.
	 */
	int childType = children[0]->getType();
	string childName = children[0]->getID();
	
	/* We can't accept input if the node is a method call */
	Node* nodePtr = table->lookup(childName);

	if(nodePtr != NULL) {
		int childNodeType = nodePtr->getNodeType();

		/* The node exists in the symbol table, is it a method? */
		if(childNodeType == FUNC || childNodeType == PROCEDURE) {
			printErrorHeader("input request");
			error_input_node_type(childName);
			isValid = 0;
		}
	}

	/* If the child type is not a number or char, we can't accept input. */
	if(childType != TNUMBER && childType != TCHAR 
			&& childType != TSTRING) {
		printErrorHeader("input request");
		error_input_type(childName, childType);
		isValid = 0;
	}

	/* Check the children using Node superclass. */
	isValid &= Node::check();

	return isValid;
}
