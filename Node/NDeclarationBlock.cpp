#include "NDeclarationBlock.hpp"
#include "TypeDefs.hpp"

NDeclarationBlock::NDeclarationBlock() {
	name = "Declaration Block";
	nodeType = DECLARATIONBLOCK;
}

NDeclarationBlock::NDeclarationBlock(Node* child) {
	children.push_back(child);
	name = "Declaration Block";
	nodeType = DECLARATIONBLOCK;
}

int NDeclarationBlock::check() {
	int isValid = 1;

	/* Check for 'hatta' entry point. */
	Node* nodePtr = table->lookup("hatta");
	if(nodePtr == NULL) {
		error_no_entry();
		isValid = 0;
	}

	/* We have a 'hatta', but is it a function declaration? */
	else if(nodePtr->getNodeType() != PROCEDURE) {
		error_no_entry();
		isValid = 0;
	}

	/* Now proceed as a generic node. */
	isValid &= Node::check();

	return isValid;
}
