#include "NDeclarationBlock.hpp"
#include "TypeDefs.hpp"

NDeclarationBlock::NDeclarationBlock() {
	name = "Declaration Block";
	nodeType = DECLARATIONBLOCK;
	isRootNode = false;
}

NDeclarationBlock::NDeclarationBlock(Node* child) {
	children.push_back(child);
	name = "Declaration Block";
	nodeType = DECLARATIONBLOCK;
	isRootNode = false;
}
/* TODO Refactor big if satement block.
 */
int NDeclarationBlock::check() {
	int isValid = 1;

	/* Check for 'hatta' entry point, if this is the topmost root node. */
	if(isRootNode) {
		Node* nodePtr = table->lookup("hatta");

		if(nodePtr == NULL) {
			error_no_entry();
			isValid = 0;
		}

		/* We have a 'hatta', but is it a procedure? */
		else if(nodePtr->getNodeType() == FUNC) {
			error_entry_not_proc();
			isValid = 0;
		}

		/* No hatta. Sad face :( */
		else if (nodePtr->getNodeType() != PROCEDURE) {
			error_no_entry();
			isValid = 0;
		}
	}

	/* Now proceed as a generic node. */
	isValid &= Node::check();

	return isValid;
}

