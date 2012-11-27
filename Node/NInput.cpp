#include "NInput.hpp"
#include "TypeDefs.hpp"
#include "../Errors/TypeMap.hpp" /* debug include - remove this! */

NInput::NInput(Node* id)
{
	name = "NInput";
	nodeType = INPUTNODE;
	children.push_back(id);
}
/* TODO Refactor this check function
 * Its logic is a little confused and should be expressed
 * more consise and clearly
 */
int NInput::check() {
	int isValid = 1;

	/* 
	 * We need to ensure that the identifier is a type that can accept
	 * input.
	 */
	int childType = children[0]->getType();
	
	/* We can't accept input if the node is a method call */
	Node* nodePtr = table->lookup(children[0]->getID());
	if(nodePtr != NULL) {
		int childNodeType = nodePtr->getNodeType();

		if(childNodeType == FUNC || childNodeType == PROCEDURE) {
			printErrorHeader("input request");
			error_input_node_type(children[0]->getID());
			isValid = 0;
		}
	}

	/* If the child type is not a number or char, we can't accept input. */
	if(childType != TNUMBER && childType != TCHAR 
			&& childType != TSTRING) {
		printErrorHeader("input request");
		error_input_type(children[0]->getID(), childType);
		isValid = 0;
	}

	/* Check the children using Node superclass. */
	isValid &= Node::check();

	return isValid;
}
